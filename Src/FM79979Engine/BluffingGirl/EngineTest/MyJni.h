#ifndef	_MY_JNI_H_
#define	_MY_JNI_H_

#ifdef __cplusplus
extern "C"{
#endif
	void	ItemPurchaseSuccess(const char*e_strProductName);
	void	ItemPurchaseFailed();
	void	SetWaitForRespond(int e_iWaitingRespond);

#ifdef __cplusplus
};
#endif

#endif