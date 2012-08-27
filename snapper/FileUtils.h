/*
 * Copyright (c) [2011-2012] Novell, Inc.
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


#ifndef SNAPPER_FILE_UTILS_H
#define SNAPPER_FILE_UTILS_H


#include <string>
#include <vector>


namespace snapper
{
    using std::string;
    using std::vector;


    class SDir
    {
    public:

	explicit SDir(const string& base_path);
	explicit SDir(const SDir& dir, const string& name);

	SDir(const SDir&);
	SDir& operator=(const SDir&);
	~SDir();

	string fullname(bool with_base_path = true) const;
	string fullname(const string& name, bool with_base_path = true) const;

	vector<string> entries() const;

	int stat(const string& name, struct stat* buf, int flags) const;
	int open(const string& name, int flags) const;
	int readlink(const string& name, string& buf) const;
	int mkdir(const string& name, mode_t mode) const;
	int unlink(const string& name, int flags) const;
	int chmod(const string& name, mode_t mode, int flags) const;
	int chown(const string& name, uid_t owner, gid_t group, int flags) const;
	int rename(const string& oldname, const string& newname) const;

    private:

	const string base_path;
	const string path;

	int dirfd;

    };


    class SFile
    {
    public:

	explicit SFile(const SDir& dir, const string& name);

	string fullname(bool with_base_path = true) const;

	int stat(struct stat* buf, int flags) const;
	int open(int flags) const;
	int readlink(string& buf) const;

    private:

	const SDir& dir;
	const string name;

    };

}


#endif