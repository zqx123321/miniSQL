#pragma once
#include "minisql.h"
enum opType;

opType INTERPRETER_GetOp();
void INTERPRETER_NewLine();
void INTERPRETER_Welcome();
void INTERPRETER_Main();
void INTERPRETER_Create();
void INTERPRETER_Select();
void INTERPRETER_Insert();
void INTERPRETER_Update();
void INTERPRETER_Delete();
void INTERPRETER_Drop();
void INTERPRETER_Execfile();

