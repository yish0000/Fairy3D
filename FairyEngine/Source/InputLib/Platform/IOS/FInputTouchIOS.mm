//
//  FInputTouchIOS.m
//  InputLib
//
//  Created by Yish on 16/3/2.
//  Copyright © 2016年 YishSoft. All rights reserved.
//

#include "FInputTouch.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <UIKit/UIGestureRecognizerSubclass.h>

#define MAX_TOUCH_POINT 16

struct FInputTouchSavedData
{
    float x;
    float y;
    uint32 tick;
};

@interface FInputTouchHookIOS : UIGestureRecognizer
{
    std::unordered_map<uptrint, FInputTouchSavedData> m_mapSavedData;
    NSTimer* m_idTimer;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event;
- (BOOL)canPreventGestureRecognizer:(UIGestureRecognizer*)preventedGestureRecognizer;
- (BOOL)canBePreventedByGestureRecognizer:(UIGestureRecognizer*)preventingGestureRecognizer;
- (void)handleGesture;
- (void)timerFired:(NSTimer*) timer;

- (id)init;

@end

class FInputTouchIOS : public FInputTouch
{
public:
    FInputTouchIOS() {}
    
    // Initialize the touch device.
    bool Init()
    {
        if( !FInputTouch::Init() )
            return false;

        UIApplication* app = [UIApplication sharedApplication];
        UIWindow* win = app.windows[0];
        [win.rootViewController.view addGestureRecognizer:[[FInputTouchHookIOS alloc] init]];
        
        m_bValid = true;
        return true;
    }
};

template <> FInputDevice* CreateInputDevice<IDT_TOUCH>()
{
    return new FInputTouchIOS();
}

@implementation FInputTouchHookIOS

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
}

- (void)handleGesture
{
}

- (BOOL)canPreventGestureRecognizer:(UIGestureRecognizer*)preventedGestureRecognizer
{
    return NO;
}

- (BOOL)canBePreventedByGestureRecognizer:(UIGestureRecognizer*)preventingGestureRecognizer
{
    return NO;
}

- (void)timerFired:(NSTimer*)timer
{
}

- (id)init
{
    id rv = [super initWithTarget:self action:@selector(handleGesture)];
    m_idTimer = [NSTimer timerWithTimeInterval:0.02 target:self selector:@selector(timerFired:) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:m_idTimer forMode:NSDefaultRunLoopMode];
    self.cancelsTouchesInView = NO;
    self.delaysTouchesBegan = NO;
    self.delaysTouchesEnded = NO;
    return rv;
}

@end