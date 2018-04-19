#import <Foundation/Foundation.h>

@interface O : NSObject

@end

@implementation O

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

@end
