package util;

import java.io.File;

public class RootUtil
{

	public static boolean isRooted() 
	{
		return findBinary("su");
	}

	private  static boolean findBinary(String binaryName)
	{
		boolean found = false;
		if (!found)
		{
			String[] places = {"/sbin/", "/system/bin/", "/system/xbin/", "/data/local/xbin/",
					"/data/local/bin/", "/system/sd/xbin/", "/system/bin/failsafe/", "/data/local/"};
			for (String where : places)
			{
				if ( new File( where + binaryName ).exists() )
				{
					found = true;
					break;
				}
			}
		}
		return found;
	}
//
}