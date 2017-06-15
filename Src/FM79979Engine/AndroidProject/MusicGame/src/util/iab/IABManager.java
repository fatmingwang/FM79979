package util.iab;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import java.util.*;
import util.Define;
//steps:
//1.DoPurchaseProducts
//2.mPurchaseFinishedListener
//3.mConsumeFinishedListener if cosumable is true
//
public class IABManager
{
	final int				m_ManagedType = 0;
	final int				m_UnmanagedType = 1;
	final int				m_SubscribeType = 2;
	//
	public native String	GetBase64EncodedPublicKey();
	//it's possible still cosume failed but just give player happy lah
	public  native void		ItemNotCosumeButNotPurchasedFillBackToPlayer(String e_strName);
	//after cosume or preume purchased
	public  native void		ItemPurchaseSuccess(String e_strProductName);
	//purchased but cosume failed,reconnect again?
	public  native void		ItemCosumeFailed();
	//connection failed or something else?
	public  native void		ItemPurchaseFailed();
	//
	public  native void		SetWaitForRespond(boolean e_b);
	public  native void		IABErrorMessage(String e_strMsg);
	private class cSKUData
	{
		boolean	m_bPurchased;
		boolean m_bCosumable;
		String	m_strName;
		String	m_strSkuID;
		public cSKUData()
		{
			m_bPurchased = false;
			m_bCosumable = false;
			m_strName ="";
			m_strSkuID = "";
		}
	}
	Inventory		m_InventoryFromCreate;
	List<cSKUData>	m_ManagedSKUList = new ArrayList<cSKUData>();
	List<cSKUData>	m_UnmanagedSKUList = new ArrayList<cSKUData>();
	//not all machine support subscribe function
	List<cSKUData>	m_SubscribeSKUList = new ArrayList<cSKUData>();
	cSKUData		m_CurrentSKUData;
	public void	AddSKUData(String e_strSKUID,String e_strName,int e_iType,boolean e_bConsumable)
	{
		cSKUData	l_cSKUData = new cSKUData();
		l_cSKUData.m_strName = e_strName;
		l_cSKUData.m_strSkuID = e_strSKUID;
		l_cSKUData.m_bCosumable = e_bConsumable;
		switch(e_iType)
		{
			case m_ManagedType:
				m_ManagedSKUList.add(l_cSKUData);
			break;
			case m_UnmanagedType:
				m_UnmanagedSKUList.add(l_cSKUData);
			break;
			case m_SubscribeType:
				m_SubscribeSKUList.add(l_cSKUData);
			break;
			default:
			break;
		}
	}
	//its possible  player buy the item and pay the money done,but not cosumed and connection is drop
	//so check cosumed last,if not give player what them bought.
	public boolean	IsItemPurchased(String e_strSKUID,String e_strName,int e_iType)
	{
		if( e_iType == m_ManagedType )
		{
			for (cSKUData l_SKUData : m_ManagedSKUList) 
			{
				if(l_SKUData.m_strName.equals(e_strName))
					return l_SKUData.m_bPurchased;
			}
		}
		else
		if( e_iType == m_UnmanagedType )
		{
			for (cSKUData l_SKUData : m_UnmanagedSKUList) 
			{
				if(l_SKUData.m_strName.equals(e_strName))
					return l_SKUData.m_bPurchased;
			}		
		}
		else
		if( e_iType == m_SubscribeType )
		{
			for (cSKUData l_SKUData : m_SubscribeSKUList) 
			{
				if(l_SKUData.m_strName.equals(e_strName))
					return l_SKUData.m_bPurchased;
			}
		}
		return false;
	}
	void	SetCurrentData(int e_iType,String e_strProductName)
	{
		m_CurrentSKUData = null;
		if( e_iType == m_ManagedType )
		{
			for (cSKUData l_SKUData : m_ManagedSKUList)
			{
				if(l_SKUData.m_strName.equals(e_strProductName))
				{
					m_CurrentSKUData = l_SKUData;
					return;
				}
			}	
		}
		else
		if( e_iType == m_UnmanagedType )
		{
			for (cSKUData l_SKUData : m_UnmanagedSKUList)
			{
				if(l_SKUData.m_strName.equals(e_strProductName))
				{
					m_CurrentSKUData = l_SKUData;
					return;
				}
			}	
		}
		else
		if( e_iType == m_SubscribeType )
		{
			for (cSKUData l_SKUData : m_SubscribeSKUList)
			{
				if(l_SKUData.m_strName.equals(e_strProductName))
				{
					m_CurrentSKUData = l_SKUData;
					return;
				}
			}	
		}
	}
	boolean	m_bIABReady = false;
	//1 for managed 2 for unmanaged 3 for subscribe
	//not all machine support subscribe function
	int m_iCurrentProductsType;
	public boolean	m_bWaitingForIABRespond = false;
	String m_strDebugAppLogShowString = "GameApp";
    // (arbitrary) request code for the purchase flow
    final int RC_REQUEST = 10001;

	boolean m_EnableDebugInfo = Define.g_bDebugTest;

    void alert(String message) 
	{
		IABErrorMessage(message);
        //AlertDialog.Builder bld = new AlertDialog.Builder(m_Activity);
        //bld.setMessage(message);
        //bld.setNeutralButton("OK", null);
        //Define.MyLog("Showing alert dialog: " + message);
        //bld.create().show();
    }
    void complain(String message)
	{
		Define.MyLog("Error: " + message);
        alert("Error: " + message);
    }

    // Enables or disables the "please wait" screen.
    void setWaitScreen(boolean set)
	{
		m_bWaitingForIABRespond = set;
		SetWaitForRespond(set);
        //findViewById(R.id.screen_main).setVisibility(set ? View.GONE : View.VISIBLE);
        //findViewById(R.id.screen_wait).setVisibility(set ? View.VISIBLE : View.GONE);
    }
	//The helper object
    IabHelper mHelper;
	Activity m_Activity;
	public IABManager(boolean e_bEnableDebugLog,Activity e_Activity)
	{
		m_EnableDebugInfo = e_bEnableDebugLog;
		m_Activity = e_Activity;
		m_iCurrentProductsType = -1;		
		m_bIABReady = false;
		//AddSKUData("android.test.purchased","79979ManagedProductCosumable",m_ManagedType,true);
		//AddSKUData("android.test.purchased","79979ManagedProductNoCosume",m_ManagedType,false);
		//AddSKUData("android.test.purchased","79979Unmanaged",m_UnmanagedType,false);
		//AddSKUData("android.test.purchased","79979Subscribe",m_SubscribeType,false);
		//AddSKUData("clairvoyant_glasses","HeartRestore",m_ManagedType,true);
	}
	public void	IABDestory()
	{
	
	}
    public boolean onActivityResult(int requestCode, int resultCode, Intent data) 
	{
        if (mHelper == null) return false;
        // Pass on the activity result to the helper for handling
        return mHelper.handleActivityResult(requestCode, resultCode, data);
    }
	//requery IAB function,it also a restore button function
	//if m_bIABReady is true.
	public void	Create()
	{
		m_bIABReady = false;
		String base64EncodedPublicKey = GetBase64EncodedPublicKey();//"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgJqo9yLr2wF1WZSn1oQtFEh8gvFGW3g8zddOslflXX2Yed+b/L3hEkabj+z/Stoom8GVUqaNNmafvNzkUKGfjarlejtRDzbanfhy923z28hzpcPlLhj1m/cPaVSl+pwN7xntRU6U7IjR5Ku5J082htUXWgUwepLM2oCURqeFH5FcDPm2cZRjPuBzlBqQSdWtwAevKH+yXNxnLufJxRw7Y03LIFg/eaW62vz5aSTLOVLsY4WuwmzXJVll8D1p8QE9SEsk1acog03V9KcHxr7wi1dDhRVckDeIUHsiRb+M6o9Nbo8sb9EqFvisZackD+w63KbEuXE/CN6129z/P3nKMwIDAQAB";
        mHelper = new IabHelper(m_Activity, base64EncodedPublicKey);
        Define.MyLog("Creating IAB helper.");
        // enable debug logging (for a production application, you should set this to false).
        mHelper.enableDebugLogging(true);
        mHelper.startSetup(
			new IabHelper.OnIabSetupFinishedListener() 
			{
				public void onIabSetupFinished(IabResult result) 
				{
					Define.MyLog("Setup finished.");
					if (!result.isSuccess()) 
					{
						// Oh noes, there was a problem.
						complain("Problem setting up in-app billing: " + result);
						return;
					}
					// Have we been disposed of in the meantime? If so, quit.
					if (mHelper == null) return;

					// IAB is fully set up. Now, let's get an inventory of stuff we own.
					Define.MyLog("Setup successful. Querying inventory.");
					//if( 1 )//query by data
					//{
						//ArrayList<String> additionalSkuList = new ArrayList<String>();
						//additionalSkuList.add("clairvoyant_glasses");
						//mHelper.queryInventoryAsync(true, additionalSkuList, mQueryFinishedListener);
					//}
					//else
					{
						mHelper.queryInventoryAsync(mGotInventoryListener);
					}
					m_bIABReady = true;
				}
			}
		);	
	}
	//=====================================================
	//
	//
	//
	//=====================================================
    // Listener that's called when we finish querying the items and subscriptions we own
    IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() 
	{
        public void onQueryInventoryFinished(IabResult result, Inventory inventory)
		{
			m_InventoryFromCreate = inventory;
            Define.MyLog("Query inventory finished.");

            // Have we been disposed of in the meantime? If so, quit.
            if (mHelper == null) return;

            // Is it a failure?
            if (result.isFailure())
			{
                complain("Failed to query inventory: " + result);
                return;
            }

            Define.MyLog("Query inventory was successful.");

            /*
             * Check for items we own. Notice that for each purchase, we check
             * the developer payload to see if it's correct! See
             * verifyDeveloperPayload().
             */

            // Do we have the premium upgrade?
			for (cSKUData l_SKUData : m_ManagedSKUList)
			{
				Purchase premiumPurchase = inventory.getPurchase(l_SKUData.m_strSkuID);
				if( premiumPurchase != null )
				{
					if( l_SKUData.m_bCosumable == false )
					{
						l_SKUData.m_bPurchased = (premiumPurchase != null && verifyDeveloperPayload(premiumPurchase));
						Define.MyLog("User is " + (l_SKUData.m_bPurchased ? "PREMIUM" : "NOT PREMIUM"));
					}
					else
					{
						mHelper.consumeAsync(inventory.getPurchase(l_SKUData.m_strSkuID), mConsumeFinishedListener);
						//this one is do need anymore because while purchased we give player hwat he should owned
						ItemNotCosumeButNotPurchasedFillBackToPlayer(l_SKUData.m_strName);
					}
				}
				else
				{
					Define.MyLog("ID:"+l_SKUData.m_strSkuID+",Name:"+l_SKUData.m_strName+" no such products");
				}
			}
			for (cSKUData l_SKUData : m_SubscribeSKUList)
			{
				Purchase premiumPurchase = inventory.getPurchase(l_SKUData.m_strSkuID);
				if( premiumPurchase != null )
				{
					l_SKUData.m_bPurchased = (premiumPurchase != null && verifyDeveloperPayload(premiumPurchase));
					Define.MyLog("User is " + (l_SKUData.m_bPurchased ? "subscribe" : "NOT subscribe"));
				}
				else
				{
					Define.MyLog(l_SKUData.m_strName+" no such products");
				}
			}
            //updateUi();
            setWaitScreen(false);
            Define.MyLog("Initial inventory query finished; enabling main UI.");
			SkuDetails	l_SkuDetails = m_InventoryFromCreate.getSkuDetails("heart10");
			if( l_SkuDetails != null )
			{
				Define.MyLog("l_SkuDetails exists");
			}
			else
				Define.MyLog("l_SkuDetails not exists");
        }
    };
    /** Verifies the developer payload of a purchase. */
    boolean verifyDeveloperPayload(Purchase p)
	{
        String payload = p.getDeveloperPayload();
		Define.MyLog(" Payload string is:"+payload);
		//check         String payload = "";
        //mHelper.launchPurchaseFlow(this, SKU_GAS, RC_REQUEST,mPurchaseFinishedListener, payload);
		//compare respond payload string data
        /*
         * TODO: verify that the developer payload of the purchase is correct. It will be
         * the same one that you sent when initiating the purchase.
         *
         * WARNING: Locally generating a random string when starting a purchase and
         * verifying it here might seem like a good approach, but this will fail in the
         * case where the user purchases an item on one device and then uses your app on
         * a different device, because on the other device you will not have access to the
         * random string you originally generated.
         *
         * So a good developer payload has these characteristics:
         *
         * 1. If two different users purchase an item, the payload is different between them,
         *    so that one user's purchase can't be replayed to another user.
         *
         * 2. The payload must be such that you can verify it even when the app wasn't the
         *    one who initiated the purchase flow (so that items purchased by the user on
         *    one device work on other devices owned by the user).
         *
         * Using your own server to store and verify developer payloads across app
         * installations is recommended.
         */

        return true;
    }
    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() 
	{
        public void onIabPurchaseFinished(IabResult result, Purchase purchase)
		{
            Define.MyLog("Purchase finished: " + result + ", purchase: " + purchase);
            // if we were disposed of in the meantime, quit.
            if (mHelper == null)
			{
				setWaitScreen(false);
				ItemPurchaseFailed();
				return;
			}

            if (result.isFailure()) 
			{
                complain("Error purchasing: " + result);
				ItemPurchaseFailed();
                setWaitScreen(false);
                return;
            }
            if (!verifyDeveloperPayload(purchase)) 
			{
                complain("Error purchasing. Authenticity verification failed.");
                setWaitScreen(false);
				ItemPurchaseFailed();
                return;
            }

            Define.MyLog("Purchase successful.");

            if (purchase.getSku().equals(m_CurrentSKUData.m_strSkuID)) 
			{
				//1 for managed 2 for unmanaged 3 for subscribe
				if( m_iCurrentProductsType == m_ManagedType )
				{
					if( !m_CurrentSKUData.m_bCosumable )
					{
						// bought the premium upgrade!
						Define.MyLog("Purchase is premium upgrade.");
						alert("Thank you for upgrading to premium!");
						m_CurrentSKUData.m_bPurchased = true;
						ItemPurchaseSuccess(m_CurrentSKUData.m_strName);
					}
					else
					{
						mHelper.consumeAsync(purchase, mConsumeFinishedListener);
					}
					setWaitScreen(false);
				}
				else
				if( m_iCurrentProductsType == m_UnmanagedType )
				{//here for all cosume
					// bought 1/4 tank of gas. So consume it.
					Define.MyLog("Purchase is consume.");
					mHelper.consumeAsync(purchase, mConsumeFinishedListener);
				}
				else
				if( m_iCurrentProductsType == m_SubscribeType )
				{
					// bought the infinite gas subscription
					Define.MyLog("Infinite gas subscription purchased.");
					alert("Thank you for subscribing to infinite gas!");
					m_CurrentSKUData.m_bPurchased = true;
					//updateUi();
					setWaitScreen(false);
				}
            }
        }
    };
    // Called when consumption is complete
    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener()
	{
        public void onConsumeFinished(Purchase purchase, IabResult result)
		{
            Define.MyLog("Consumption finished. Purchase: " + purchase + ", result: " + result);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            // We know this is the "gas" sku because it's the only one we consume,
            // so we don't check which sku was consumed. If you have more than one
            // sku, you probably should check...
            if (result.isSuccess()) 
			{
                // successfully consumed, so we apply the effects of the item in our
                // game world's logic, which in our case means filling the gas tank a bit
                Define.MyLog("Consumption successful. Provisioning.");
                //saveData();
				alert("Consumption successful. Provisioning.");
				m_CurrentSKUData.m_bPurchased = true;
				String l_strPayload = purchase.getDeveloperPayload();
				Define.MyLog("pay load:"+l_strPayload);
				if( l_strPayload.equals("79979") )
				{
					Define.MyLog("pay load:matched");
					//if( m_InventoryFromCreate!= null )
					{
						Define.MyLog("m_InventoryFromCreate exists");
						String l_strSKUName = purchase.getSku();
						Define.MyLog("SkuDetailsName:"+l_strSKUName);
						//SkuDetails	l_SkuDetails = m_InventoryFromCreate.getSkuDetails(l_strSKUName);
						//if( l_SkuDetails != null )
						//{
							//Define.MyLog("SkuDetails exists");
							//Define.MyLog("SkuDetails price:"+l_SkuDetails.mPrice);
							//if( !l_SkuDetails.mPrice.equals("0") )
								ItemPurchaseSuccess(m_CurrentSKUData.m_strName);
						//}
					}
				}
            }
            else
			{
                complain("Error while consuming: " + result);
				ItemCosumeFailed();
            }
            //updateUi();
            Define.MyLog("End consumption flow.");
        }
    };
	//1 for managed 2 for unmanaged 3 for subscribe
	//
	public void	DoPurchaseProducts(int e_iProductsType,String e_strProductsName)
	{
		if( m_bWaitingForIABRespond == false )
		{
			setWaitScreen(true);
			if( m_EnableDebugInfo )
			{
				String	l_strDebugInfo = "productType:"+e_iProductsType+",ProductName:"+e_strProductsName;
				Define.MyLog(l_strDebugInfo);
			}
			m_iCurrentProductsType = e_iProductsType;
			SetCurrentData(e_iProductsType,e_strProductsName);
			if( m_CurrentSKUData == null )
			{
				complain(e_strProductsName+" not exists!");
				setWaitScreen(false);
				return;
			}
			String payload = "79979";
			if( e_iProductsType == m_ManagedType )
			{
				if ( m_CurrentSKUData.m_bPurchased && m_CurrentSKUData.m_bCosumable == false )
				{
					setWaitScreen(false);
					complain("No need! You're subscribed to infinite gas. Isn't that awesome?");
					return;
				}
				mHelper.launchPurchaseFlow(m_Activity,m_CurrentSKUData.m_strSkuID, RC_REQUEST,mPurchaseFinishedListener, payload);
			}
			else
			if( e_iProductsType == m_UnmanagedType )
			{
				mHelper.launchPurchaseFlow(m_Activity,m_CurrentSKUData.m_strSkuID, RC_REQUEST,mPurchaseFinishedListener, payload);
			}
			else
			if( e_iProductsType == m_SubscribeType )
			{
				if (!mHelper.subscriptionsSupported()) 
				{
					complain("Subscriptions not supported on your device yet. Sorry!");
					setWaitScreen(false);
					return;
				}
				if (m_CurrentSKUData.m_bPurchased) 
				{
					complain("No need! You're subscribed to infinite gas. Isn't that awesome?");
					setWaitScreen(false);
					return;
				}
				mHelper.launchPurchaseFlow(m_Activity,m_CurrentSKUData.m_strSkuID, IabHelper.ITEM_TYPE_SUBS,RC_REQUEST, mPurchaseFinishedListener, payload);
			}
			else
				alert("not support IAB type(only vaild 1,2,3)") ;
		}
	}
}