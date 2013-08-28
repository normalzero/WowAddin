#include "stdafx.h"

BOOL CCommand_TestCommand(char const* cmd, char const* args)
{
    Console::Write("Hello from TestCommand: cmd %s, args %s", INPUT_COLOR, cmd, args);

    uint64 guid = ObjectMgr::GetActivePlayerGuid();
    CGObject_C *pPlayer = ObjectMgr::GetObjectPtr(guid, TYPEMASK_PLAYER);

    if (!pPlayer)
    {
        Console::Write("TestCommand: Not in world!", ERROR_COLOR);
        return TRUE;
    }

    C3Vector pos;
    pPlayer->GetPosition(pos);

    const char *model;
    BOOL result = pPlayer->GetModelFileName(&model);

    Console::Write("Local player: position %f %f %f, model %s", ECHO_COLOR, pos.X, pos.Y, pos.Z, result ? model : "Unknown");

    CGObject_C *pTarget = ObjectMgr::GetObjectPtr(pPlayer->GetValue<uint64>(UNIT_FIELD_TARGET), TYPEMASK_UNIT);

    if (pTarget)
        Console::Write("Target %s, guid 0x%016llX", ECHO_COLOR, pTarget->GetObjectName(), pTarget->GetValue<uint64>(OBJECT_FIELD_GUID));
    else
        Console::Write("No target!", ECHO_COLOR);

    return TRUE;
}

BOOL CCommand_Beastmaster(char const* cmd, char const* args)
{
    CDataStore data(CMSG_BEASTMASTER);
    int state = _strnicmp(args, "off", INT_MAX) != 0;
    data.PutInt8(state);
    ClientServices::SendPacket(&data);

    Console::Write("Beastmaster mode is %s", ECHO_COLOR, state ? "on" : "off");

    return TRUE;
}

BOOL CCommand_Invis(char const* cmd, char const* args)
{
    CDataStore data(CMSG_GM_INVIS);
    int state = _strnicmp(args, "off", INT_MAX) != 0;
    data.PutInt32(state);
    ClientServices::SendPacket(&data);

    Console::Write("GM invis mode is %s", ECHO_COLOR, state ? "on" : "off");

    return TRUE;
}

BOOL CCommand_DBLookup(char const* cmd, char const* args)
{
    CDataStore data(CMSG_DBLOOKUP);
    data.PutString(args);
    ClientServices::SendPacket(&data);

    return TRUE;
}

BOOL ShowObjectsCallback(uint64 objectGuid, void *param)
{
    CGObject_C *pObject = ObjectMgr::GetObjectPtr(objectGuid, TYPEMASK_OBJECT);

    if (pObject)
        Console::Write("Object %s, guid 0x%016llX", ECHO_COLOR, pObject->GetObjectName(), pObject->GetValue<uint64>(OBJECT_FIELD_GUID));

    return TRUE;
}

BOOL CCommand_ShowObjects(char const* cmd, char const* args)
{
    ObjectMgr::EnumVisibleObjects(ShowObjectsCallback, NULL);

    return TRUE;
}
