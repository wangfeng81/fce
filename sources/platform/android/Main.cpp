/*
**
**	Main entry.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include "../../../interface/platform/AppAgent.h"


/*
 ! Android native activity 创建入口
 @ activity			原生activity
 @ savedState		状态缓存
 @ savedStateSize	状态缓存大小
 */
__attribute__ ((visibility ("default")))
void ANativeActivity_onCreate( ANativeActivity* anActivity,
							void* savedState, size_t savedStateSize)
{
	typedef void ( *PAndroidMain )( ANativeActivity*, void*, size_t );

//android.permission.INSTALL_PACKAGES 
//允许一个程序安装packages，通过这个方式来更新
	// 如果获取路径为 null，就用 jni 获取
	void* libFC_Engine = dlopen("/data/data/com.demonhunter/lib/libFcEngine.so",RTLD_NOW);
	void* libAndroidMain = dlopen("/data/data/com.demonhunter/lib/libFcEditor.so",RTLD_NOW);

	PAndroidMain AndroidMain = ( PAndroidMain )dlsym( libAndroidMain, "AndroidMain" );

	AndroidMain( anActivity, savedState, savedStateSize );
}