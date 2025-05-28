/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "controllers/GameController.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

#include "AppDelegate.h"
#include "controllers/GameController.h"  // ������Ϸ������
#include "cocos2d.h"

USING_NS_CC;

// ��Ʒֱ��ʺʹ��ڴ�С
static const cocos2d::Size DESIGN_RESOLUTION = cocos2d::Size(1080, 2080);

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    // ���û�� OpenGL ��ͼ�������µĴ��ڣ�ָ�� 1080x2080 ���ڴ�С��
    if (!glview) {
        glview = GLViewImpl::createWithRect(
            "CardDemo",              // ���ڱ���
            cocos2d::Rect(0, 0, 1080, 2080),  // ���ڴ�С
            0.5f                           // �� DPI ���ţ���ѡ���������������
        );
        director->setOpenGLView(glview);
    }

    // ������Ʒֱ���Ϊ 1080x2080
    glview->setDesignResolutionSize(
        DESIGN_RESOLUTION.width,
        DESIGN_RESOLUTION.height,
        ResolutionPolicy::FIXED_WIDTH  // �̶�������䣬�߶��Զ�����
    );

    // �ر�Ĭ�ϵ� FPS ��ʾ����ѡ��������ʾ�뱣�� director->setDisplayStats(true);��
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    // ע��ȫ�ֹ��߻��������Դ������������ģ��ȣ�
    // register_all_packages();  // ���ʹ����Դ����������ȡ��ע��

    // ������������Ϸ���������滻Ĭ�ϵ� HelloWorld ������
    auto gameController = GameController::createInstance();
    director->runWithScene(gameController);

    return true;
}



// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
