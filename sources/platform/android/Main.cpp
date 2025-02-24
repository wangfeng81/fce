/*
**
**	Main entry.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include "../../../interface/platform/AppAgent.h"


/*
 ! Android native activity �������
 @ activity			ԭ��activity
 @ savedState		״̬����
 @ savedStateSize	״̬�����С
 */
__attribute__ ((visibility ("default")))
void ANativeActivity_onCreate( ANativeActivity* anActivity,
							void* savedState, size_t savedStateSize)
{
	typedef void ( *PAndroidMain )( ANativeActivity*, void*, size_t );

//android.permission.INSTALL_PACKAGES 
//����һ������װpackages��ͨ�������ʽ������
	// �����ȡ·��Ϊ null������ jni ��ȡ
	void* libFC_Engine = dlopen("/data/data/com.demonhunter/lib/libFcEngine.so",RTLD_NOW);
	void* libAndroidMain = dlopen("/data/data/com.demonhunter/lib/libFcEditor.so",RTLD_NOW);

	PAndroidMain AndroidMain = ( PAndroidMain )dlsym( libAndroidMain, "AndroidMain" );

	AndroidMain( anActivity, savedState, savedStateSize );
}