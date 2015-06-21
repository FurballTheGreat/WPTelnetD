#pragma once 

#include "pch.h"
#include<string>
#include "AclHelpers.h"

#include "Console.h"

void PrintOwnerInformation(IConsole *pConsole, PSECURITY_DESCRIPTOR pDescriptor);
void PrintAclInformation(IConsole *pConsole, PSECURITY_DESCRIPTOR pDescriptor, PACL pAcl);