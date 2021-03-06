/*
 * Copyright (c) 2012 Novell, Inc.
 * Copyright (c) 2016 SUSE LLC
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#include <string>
#include <list>
#include <map>

using std::string;
using std::list;
using std::map;

#include "dbus/DBusMessage.h"
#include "dbus/DBusConnection.h"
#include "snapper/Snapshot.h"
#include "snapper/File.h"
#include "snapper/SnapperTmpl.h"

using namespace snapper;


struct XConfigInfo
{
    string config_name;
    string subvolume;

    map<string, string> raw;

    template<typename Type>
    void read(const char* name, Type& value)
    {
	map<string, string>::const_iterator pos = raw.find(name);
	if (pos != raw.end())
	    pos->second >> value;
    }
};


struct XSnapshot
{
    SnapshotType getType() const { return type; }

    unsigned int getNum() const { return num; }
    bool isCurrent() const { return num == 0; }

    time_t getDate() const { return date; }

    uid_t getUid() const { return uid; }

    unsigned int getPreNum() const { return pre_num; }

    const string& getDescription() const { return description; }

    const string& getCleanup() const { return cleanup; }

    const map<string, string>& getUserdata() const { return userdata; }

    SnapshotType type;
    unsigned int num;
    time_t date;
    uid_t uid;
    unsigned int pre_num;
    string description;
    string cleanup;
    map<string, string> userdata;
};


struct XSnapshots
{
    typedef list<XSnapshot>::iterator iterator;
    typedef list<XSnapshot>::const_iterator const_iterator;

    iterator begin() { return entries.begin(); }
    const_iterator begin() const { return entries.begin(); }

    iterator end() { return entries.end(); }
    const_iterator end() const { return entries.end(); }

    const_iterator find(unsigned int num) const;

    iterator findPre(iterator post);
    const_iterator findPre(const_iterator post) const;

    iterator findPost(iterator pre);
    const_iterator findPost(const_iterator pre) const;

    iterator erase(iterator pos) { return entries.erase(pos); }

    list<XSnapshot> entries;
};


struct XFile
{
    string name;
    unsigned int status;
};


struct XQuotaData
{
    uint64_t size;
    uint64_t used;
};


namespace DBus
{

    template <> struct TypeInfo<XSnapshot> { static const char* signature; };
    template <> struct TypeInfo<XConfigInfo> { static const char* signature; };
    template <> struct TypeInfo<XFile> { static const char* signature; };

    Hihi& operator>>(Hihi& hihi, XConfigInfo& data);

    Hihi& operator>>(Hihi& hihi, SnapshotType& data);
    Hoho& operator<<(Hoho& hoho, SnapshotType data);

    Hihi& operator>>(Hihi& hihi, XSnapshot& data);

    Hihi& operator>>(Hihi& hihi, XFile& data);

    Hihi& operator>>(Hihi& hihi, XQuotaData& data);

}
