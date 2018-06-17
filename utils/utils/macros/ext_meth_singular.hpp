#pragma once

#ifndef __UTILITIES__EXTENSION_METHODS__SINGULAR__
#define __UTILITIES__EXTENSION_METHODS__SINGULAR__
#define EXTENSION_METHOD_DOC_PRE(name) \
	struct name##__t{};

#define EXTENSION_METHOD_CUSTOM_OP(preModifiers_ReturnType,name,typeToExtend,op)\
	struct name##__t{};\
	const name##__t name{};\
	preModifiers_ReturnType operator op(typeToExtend,const name##__t)

#define EXTENSION_METHOD_DECL_CUSTOM_OP(preModifiers_ReturnType,name,typeToExtend,op)\
	preModifiers_ReturnType operator op(typeToExtend,const name##__t)

#define EXTENSION_METHOD_DOC_CUSTOM_OP(preModifiers_ReturnType,name,typeToExtend,op)\
	const name##__t name{};\
	EXTENSION_METHOD_DECL_CUSTOM_OP(preModifiers_ReturnType,name,typeToExtend,op)



#define EXTENSION_METHOD(preModifiers_ReturnType,name,typeToExtend)EXTENSION_METHOD_CUSTOM_OP(preModifiers_ReturnType,name,typeToExtend,%)

#define EXTENSION_METHOD_DECL(preModifiers_ReturnType,name,typeToExtend)\
	EXTENSION_METHOD_DECL_CUSTOM_OP(preModifiers_ReturnType,name,typeToExtend,%)


#define EXTENSION_METHOD_DOC(preModifiers_ReturnType,name,typeToExtend)\
	EXTENSION_METHOD_DOC_CUSTOM_OP(preModifiers_ReturnType,name,typeToExtend,%)  
#endif // !__UTILITIES__EXTENSION_METHODS__SINGULAR__

