#include "pch.h"
#include "AclHelpers.h"

#include "Console.h"
#include "TerminalHelper.h"

std::string GetSidName(PSID  pSid) {
	/*if (!pSid)
		return "Unknown";*/
	wchar_t username[1024] = L"";
	wchar_t domain[1024] = L"";

	ULONG len_username = sizeof(username);
	ULONG len_domain = sizeof(domain);
	
	SID_NAME_USE sid_name_use;
	std::wstring result = L"unknown";
	if (LookupAccountSidW(NULL, pSid,
		username, &len_username, domain, &len_domain, &sid_name_use)) {
		result = domain + std::wstring(L"\\") + username;
	}
	else {
#ifdef PHONE
		LPSTR str;
		ConvertSidToStringSidA(pSid,&str);

		std::string sidstr = str;
		LocalFree(str);
		return sidstr;
#endif
	}

	return std::string(result.begin(), result.end());
}

void PrintOwnerInformation(IConsole *pConsole, PSECURITY_DESCRIPTOR pDescriptor){
	BOOL ownerDefaulted = FALSE;
	PSID ownerSid = NULL;
	TerminalHelper helper(pConsole);
	pConsole->WriteLine("");
	if (!GetSecurityDescriptorOwner(pDescriptor, &ownerSid, &ownerDefaulted))
	{
		pConsole->Write("GetSecurityDescriptorOwner: ");
		pConsole->WriteLine(GetLastErrorAsString());
	}
	else
		helper.WriteUnderlined("Owner: " + GetSidName(ownerSid));	
}

std::string GetAceTypeName(BYTE pAceType){
	switch (pAceType) {
	case  ACCESS_ALLOWED_ACE_TYPE                :
		return "ACCESS_ALLOWED_ACE_TYPE";
	case  ACCESS_DENIED_ACE_TYPE                 :
		return "ACCESS_DENIED_ACE_TYPE";
	case  SYSTEM_AUDIT_ACE_TYPE                  :
		return "SYSTEM_AUDIT_ACE_TYPE";
	case  SYSTEM_ALARM_ACE_TYPE                  :
		return "ACCESS_ALLOWED_ACE_TYPE";		
	case  ACCESS_ALLOWED_OBJECT_ACE_TYPE         :
		return "ACCESS_ALLOWED_OBJECT_ACE_TYPE";
	case  ACCESS_DENIED_OBJECT_ACE_TYPE          :
		return "ACCESS_DENIED_OBJECT_ACE_TYPE";
	case  SYSTEM_AUDIT_OBJECT_ACE_TYPE           :
		return "SYSTEM_AUDIT_OBJECT_ACE_TYPE";
	case  SYSTEM_ALARM_OBJECT_ACE_TYPE           :
		return "SYSTEM_ALARM_OBJECT_ACE_TYPE";
	case  ACCESS_ALLOWED_CALLBACK_ACE_TYPE       :
		return "ACCESS_ALLOWED_CALLBACK_ACE_TYPE";
	case  ACCESS_DENIED_CALLBACK_ACE_TYPE        :
		return "ACCESS_DENIED_CALLBACK_ACE_TYPE";
	case  ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE:
		return "ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE";
	case  ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE :
		return "ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE";
	case  SYSTEM_AUDIT_CALLBACK_ACE_TYPE         :
		return "SYSTEM_AUDIT_CALLBACK_ACE_TYPE";
	case  SYSTEM_ALARM_CALLBACK_ACE_TYPE         :
		return "SYSTEM_ALARM_CALLBACK_ACE_TYPE";
	case  SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE  :
		return "SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE";
	case  SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE  :
		return "SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE";
	case  SYSTEM_MANDATORY_LABEL_ACE_TYPE        :
		return "SYSTEM_MANDATORY_LABEL_ACE_TYPE";
	case  SYSTEM_SCOPED_POLICY_ID_ACE_TYPE       :
		return "SYSTEM_SCOPED_POLICY_ID_ACE_TYPE";
	}
}

void PrintAclInformation(IConsole *pConsole, PSECURITY_DESCRIPTOR pDescriptor, PACL pAcl){
	ACL_SIZE_INFORMATION aclSize = { 0 };
	if (pAcl != NULL){
		if (!GetAclInformation(pAcl, &aclSize, sizeof(aclSize),
			AclSizeInformation)){
			return;
		}
	}
	for (ULONG acl_index = 0; acl_index < aclSize.AceCount; acl_index++){
		ACCESS_ALLOWED_ACE* ace;

		if (GetAce(pAcl, acl_index, (PVOID*)&ace))
		{
			std::string name = GetSidName((PSID)(&(ace->SidStart)));
			pConsole->WriteLine("Subject: "+name);
			pConsole->WriteLine("Type:    "+GetAceTypeName(ace->Header.AceType));
			pConsole->Write("Rights:  ");
			
			int count = 0;
#define PrintRight(right) {if(count>4) {pConsole->WriteLine("");pConsole->Write("         ");count=0;} count++;pConsole->Write(right);}
			
			if (ace->Mask == 2032127){
				PrintRight("Full Control (All)");
			}
			else if (ace->Mask == 1179817){
				PrintRight("Read(RX)");
			}
			else if (ace->Mask == 1180086) {
				PrintRight("Add");
			}
			else if (ace->Mask == 1180095){
				PrintRight("Add & Read");
			}
			else if (ace->Mask == 1245631){
				PrintRight("Change");
			}
			else{

				if (ace->Mask & 1)
					PrintRight("ACCESS_READ ");
				if (ace->Mask & 2)
					PrintRight("ACCESS_WRITE ");
				if (ace->Mask & 4)
					PrintRight("ACCESS_CREATE ");
				if (ace->Mask & 8)
					PrintRight("ACCESS_EXEC ");
				if (ace->Mask & 16)
					PrintRight("ACCESS_DELETE ");
				if (ace->Mask & 32)
					PrintRight("ACCESS_ATTRIB ");
				if (ace->Mask & 0x40)
					PrintRight("ACCESS_PERM ");
				if (ace->Mask & 0x8000)
					PrintRight("ACCESS_GROUP ");
				if (ace->Mask & DELETE)
					PrintRight("DELETE ");
				if (ace->Mask & READ_CONTROL)
					PrintRight("READ_CONTROL ");

				if (ace->Mask & WRITE_DAC)
					PrintRight("WRITE_DAC ");
				if (ace->Mask & WRITE_OWNER)
					PrintRight("WRITE_OWNER ");
				if (ace->Mask & SYNCHRONIZE)
					PrintRight("SYNCHRONIZE ");
				if (ace->Mask & ACCESS_SYSTEM_SECURITY)
					PrintRight("ACCESS_SYSTEM_SECURITY ");

				if (ace->Mask & MAXIMUM_ALLOWED)
					PrintRight("MAXIMUM_ALLOWED ");
				if (ace->Mask & GENERIC_ALL)
					PrintRight("GENERIC_ALL ");
				if (ace->Mask & GENERIC_EXECUTE)
					PrintRight("GENERIC_EXECUTE ");
				if (ace->Mask & GENERIC_WRITE)
					PrintRight("GENERIC_WRITE ");
				if (ace->Mask & GENERIC_READ)
					PrintRight("GENERIC_READ ");
				if (ace->Mask & SPECIFIC_RIGHTS_ALL)
					PrintRight("SPECIFIC_RIGHTS_ALL ");
				if (ace->Mask & STANDARD_RIGHTS_REQUIRED)
					PrintRight("STANDARD_RIGHTS_REQUIRED ");
				if (ace->Mask & STANDARD_RIGHTS_ALL)
					PrintRight("STANDARD_RIGHTS_ALL ");
			}
			pConsole->WriteLine("");
			pConsole->WriteLine("");
		}
	}
}

