#import <Foundation/Foundation.h>

#if CGFLOAT_IS_DOUBLE
#define MZRound(x)  round(x)
#else
#define MZRound(x)  roundf(x)
#endif

@interface O : NSObject

@end

@implementation O
{
  int _age;
  NSTimeInterval _duration;
}

- (id)init
{
    self = [super init];
    if (self) {
        int a = (int)self;
        if (a % 2) {
            goto exit;
            a++;
        }
    exit:

    }
    return self;
}

- (void)setDuration:(NSTimeInterval)duration{
    if (_duration != duration){
        _duration = duration;

        NSInteger time = MZRound(duration);
        NSString *title = [NSString stringWithFormat:@"%02zd:%02zd", time / 60, time % 60];
        [self setTitle:title forState:UIControlStateNormal];
    }
}@end
