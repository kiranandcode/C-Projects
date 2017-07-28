#ifndef BASE_H
#define BASE_H

// exported variables
extern char Look;


// exported functions
extern void GetChar();
extern void Error(char *string);
extern void Abort(char *string);
extern void Expected(char *string);
extern void Match(char c);
extern int IsAlpha(char c);
extern int IsDigit(char c);
extern char GetName();
extern char GetNum();
extern void Emit(char *string);
extern void EmitLn(char *string);
extern void Init();
extern int IsAddop(char c);


#endif // base.h



