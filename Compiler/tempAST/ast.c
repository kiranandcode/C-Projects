typedef struct Identifier *Identifier;
typedef struct QualifiedIdentifier  *QualifiedIdentifier; 
typedef struct QualifiedIdentifierList *QualifiedIdentifierList;
typedef struct CompilationUnit *CompilationUnit;
typedef struct ImportDeclaration *ImportDeclaration;
typedef struct TypeDeclaration *TypeDeclaration;
typedef struct ClassOrInterfaceDeclaration *ClassOrInterfaceDeclaration;
typedef struct ClassDeclaration *ClassDeclaration;
typedef struct InterfaceDeclaration *InterfaceDeclaration;
typedef struct NormalClassDeclaration *NormalClassDeclaration;
typedef struct EnumDeclaration *EnumDeclaration;
typedef struct TypeParameters *TypeParameters; 
typedef struct TypeList *TypeList;
typedef struct InterfaceBody *InterfaceBody;
typedef struct Type *Type;
typedef struct ClassBody *ClassBody;
typedef struct EnumBody *EnumBody;
typedef struct BasicType *BasicType;
typedef struct ReferenceType *ReferenceType;
typedef struct TypeArguments *TypeArguments;
typedef struct TypeArgument *TypeArgument;
typedef struct NonWildcardTypeArguments *NonWildcardTypeArguments;
typedef struct TypeParameter *TypeParameter;
typedef struct Bound *Bound;
typedef struct ElementValuePair *ElementValuePair;
typedef struct ElementValue *ElementValue;
typedef struct Expression1 *Expression1;
typedef struct ElementValueInitializer *ElementValueInitializer;
typedef struct ElementValues *ElementValues;
typedef struct ClassBodyDeclaration *ClassBodyDeclaration;
typedef struct Modifier *Modifier;
typedef struct MemberDecl *MemberDecl;
typedef struct Block *Block;
typedef struct MethodOrFieldDecl *MethodOrFieldDecl;
typedef struct VoidMethodDeclaratorRest *VoidMethodDeclaratorRest;
typedef struct ConstructorDeclaratorRest *ConstructorDeclaratorRest;
typedef struct GenericMethodOrConstructorDecl *GenericMethodOrConstructorDecl;
typedef struct MethodOrFieldRest *MethodOrFieldRest;
typedef struct FieldDeclaratorsRest *FieldDeclaratorsRest;
typedef struct MethodDeclaratorsRest *MethodDeclaratorsRest;
typedef struct VariableDeclaratorRest *VariableDeclaratorRest;
typedef struct VariableDeclarator *VariableDeclarator;
typedef struct FormalParameters *FormalParameters;
typedef struct GenericMethodOrConstructorRest *GenericMethodOrConstructorRest;
typedef struct MethodDeclaratorRest *MethodDeclaratorRest;
typedef struct InterfaceMemberDecl *InterfaceMemberDecl;
typedef struct InterfaceBodyDeclaration *InterfaceBodyDeclaration;

struct Identifier {
	char * identifier;
};


struct QualifiedIdentifier {
	unsigned int count;
	Identifier ident;
	Identifier *others;
};


struct QualifiedIdentifierList {
	unsigned int count;
	QualifiedIdentifier ident;
	QualifiedIdentifier *others;
};
	
struct CompilationUnit {
	// package
	QualifiedIdentifier ident;
	//;
	unsigned int import_count;
	ImportDeclaration *import;
	unsigned int type_count;
	TypeDeclaration *type;
};

struct ImportDeclaration {
	unsigned int isstatic;
	unsigned int isstarred;
	unsigned int count;
	Identifier ident;
	Identifier *others;
};

struct TypeDeclaration {
	ClassOrInterfaceDeclaration *decl;
};


struct ClassOrInterfaceDeclaration {
	unsigned int modifier_count;
	enum {
		CLASS_DECL,
		INTERFACE_DECL
	} classorinterface_desc;
	union {
		ClassDeclaration class_decl;
		InterfaceDeclaration interface_decl;
	} classorinterface_val;
};

struct ClassDeclaration {
	enum {
		NORMALCLASS_DECL,
		ENUM_DECL
	} classorenum_desc;
	union {
		NormalClassDeclaration class_decl;
		EnumDeclaration enum_decl;
	} classorenum_val;
};

struct InterfaceDeclaration {
	//interface
	Identifier ident;
	TypeParameters params;
	TypeList typelist;
	InterfaceBody body;
};

struct NormalClassDeclaration {
	//class
	Identifier ident;
	TypeParameters params;
	// extends
	Type extended_type;
	TypeList typelist;
	ClassBody body;
};

struct EnumDeclaration {
	//enum
	Identifier ident;
	// implements
	TypeList typelist;
	EnumBody body;
};

struct Type {
	unsigned int isarray;
	enum {
		BASIC_TYPE,
		REFERENCE_TYPE
	} basicorreferencetype_desc;
	union {
		BasicType basictype;
		ReferenceType referencetype;
	} basicorreferencetype_val;
};

struct BasicType {
	enum {
		BYTE,
		SHORT,
		CHAR,
		INT,
		LONG,
		FLOAT,
		DOUBLE,
		BOOLEAN
	} basictype;
};

struct ReferenceType {
	Identifier ident;
	TypeArguments args;
	unsigned int count;
	struct ReferenceType_ext {
		Identifier ident;
		TypeArguments args;
	} *others;
};

struct TypeArguments {
	unsigned int count;
	TypeArgument args;
	TypeArgument *others;
};
	

struct TypeArgument {
	unsigned int generic;
	// ?
	enum {
		ARG_EXTENDS,
		ARG_SUPER
	} extendsorsuper;
	ReferenceType type;
};


struct NonWildcardTypeArguments {
	TypeList typelist;
};

struct TypeArgumentsOrDiamond {
	TypeArguments args;
};

struct NonWildcardTypeArgumentsOrDiamond {
	NonWildcardTypeArguments args;
};

struct TypeParameters {
	unsigned int count;
	TypeParameter param;
	TypeParameter *others;
};

struct TypeParameter {
	Identifier Ident;
	Bound bound;
};

struct Bound {
	unsigned int count;
	ReferenceType type;
	ReferenceType *others;
};

struct Modifier {
	enum {
		MOD_PUBLIC,
		MOD_PROTECTED,
		MOD_PRIVATE,
		MOD_STATIC,
		MOD_ABSTRACT,
		MOD_FINAL,
		MOD_NATIVE,
		MOD_SYNCHRONIZED,
		MOD_TRANSIENT,
		MOD_VOLATILE,
		MOD_STRICTFP
	} modifier;
};

struct ElementValuePairs {
	unsigned int count;
	ElementValuePair pair;
	ElementValuePair *others;
};

struct ElementValuePair {
	Identifier ident;
	ElementValue value;
};

struct ElementValue {
	enum {
		ELEM_EXPR,
		ELEM_ARRAYINIT
	} expressionorarray_desc;
	union {
		Expression1 expression;
		ElementValueInitializer initializer;
	} expressionorarray_val;

};

struct ElementValueArrayInitializer {
	unsigned int count;
	struct ElementValueInitializer_ext {
		ElementValues val;
	} others;
};

struct ElementValues {
	unsigned int count;
	ElementValue value;
	ElementValue *others;
};

struct ClassBody {
	unsigned int count;
	ClassBodyDeclaration *decl;
};

struct ClassBodyDeclaration {
	enum {
		CLBOD_EMPTY,
		CLBOD_MEMBER,
		CLBOD_STATIC
	} memberorstatic_desc;
	union {
		struct {
			Modifier modifier;
			MemberDecl decl;
		} memberdecl;
		struct {
			unsigned int isstatic;
			Block block;
		} staticblock;
	} memberorstatic_val;
};

struct MemberDecl {
	enum {
		MEM_METHODORFIELD,
		MEM_IDENTVOIDMETHODDECL,
		MEM_CONSTRUCTDECL,
		MEM_GENERICMETH,
		MEM_CLASSDECL,
		MEM_INTERFACEDECL
	} memberdecl_desc;
	union {
		MethodOrFieldDecl methodorfielddecl;
		struct {
			//void
			Identifier ident;
			VoidMethodDeclaratorRest rest;
		} voidmethoddeclarator;
		struct {
			Identifier ident;
			ConstructorDeclaratorRest rest;
		} constructordeclarator;
		GenericMethodOrConstructorDecl genericmethoddecl;
		ClassDeclaration classdecl;
		InterfaceDeclaration interfacedecl;
	} memberdecl_val;
};

struct MethodOrFieldDecl {
	Type type;
	Identifier ident;
	MethodOrFieldRest rest;
};

struct MethodOrFieldRest {
	enum {
		FIELD_DECL,
		METH_DECL
	} methodorfieldrest_desc;
	union {
		FieldDeclaratorsRest fielddeclaratorsrest;
		MethodDeclaratorsRest methoddeclaratorsrest;
	} methodorfieldrest_val;
};

struct FieldDeclaratorsRest {
	unsigned int count;
	VariableDeclaratorRest vardecl;
	VariableDeclarator *others;
};

struct MethodDeclaratorRest {
	unsigned int isarray;
	FormalParameters params;
	QualifiedIdentifierList list;
	Block block;
};

struct VoidMethodDeclaratorRest {
	FormalParameters params;
	QualifiedIdentifierList list;
	Block block;
};

struct ConstructorDeclaratorRest {
	FormalParameters params;
	QualifiedIdentifierList list;
	Block block;
};

struct GenericMethodOrConstructorDecl {
	TypeParameters params;
	GenericMethodOrConstructorRest rest;
};

struct GenericMethodOrConstructorRest {
	enum {
		GENERICMETHOD,
		CONSTRUCTOR
	} genericmethodorconstructorrest_desc;
	union {
		struct {
			Type type;
			Identifier Ident;
			MethodDeclaratorRest rest;
		} genericmethod;
		struct {
			Identifier ident;
			ConstructorDeclaratorRest rest;
		} constructor;
	} genericmethodorconstructorrest_val;
};

struct InterfaceBody {
	unsigned int count;
	InterfaceBodyDeclaration *rest;
};

struct InterfaceBodyDeclaration {
	InterfaceMemberDecl decl;
	unsigned int count;
	Modifier *others;
};

	
