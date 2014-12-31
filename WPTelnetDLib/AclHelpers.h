#pragma once 

#include "pch.h"
#include<string>
#include "AclHelpers.h"
#include "wpkernel.h"
#include "Networking.h"

void PrintOwnerInformation(Connection *pConnection, PSECURITY_DESCRIPTOR pDescriptor);
void PrintAclInformation(Connection *pConnection, PSECURITY_DESCRIPTOR pDescriptor, PACL pAcl);