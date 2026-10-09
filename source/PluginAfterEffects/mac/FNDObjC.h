#ifndef _FNDObjC_h_
#define _FNDObjC_h_

#ifndef OBJC_PREFIX
#error OBJC_PREFIX must be defined
#endif

#define FND_CONCAT(x, y) FND_CONCAT_EXPANDED(x, y)
#define FND_CONCAT_EXPANDED(x, y) x ## y
#define FND_OBJC(c) FND_CONCAT(OBJC_PREFIX, c)

#endif
