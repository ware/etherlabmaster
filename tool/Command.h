/*****************************************************************************
 *
 *  $Id$
 *
 *  Copyright (C) 2006-2009  Florian Pose, Ingenieurgemeinschaft IgH
 *
 *  This file is part of the IgH EtherCAT Master.
 *
 *  The IgH EtherCAT Master is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License version 2, as
 *  published by the Free Software Foundation.
 *
 *  The IgH EtherCAT Master is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the IgH EtherCAT Master; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  ---
 *
 *  The license mentioned above concerns the source code only. Using the
 *  EtherCAT technology and brand is only permitted in compliance with the
 *  industrial property and similar rights of Beckhoff Automation GmbH.
 *
 ****************************************************************************/

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <stdexcept>
#include <vector>
#include <list>
#include <sstream>
using namespace std;

#include "../master/ioctl.h"

class MasterDevice;

/****************************************************************************/

class InvalidUsageException:
    public runtime_error
{
    friend class Command;

    protected:
        /** Constructor with stringstream parameter. */
        InvalidUsageException(
                const stringstream &s /**< Message. */
                ): runtime_error(s.str()) {}
};

/****************************************************************************/

class CommandException:
    public runtime_error
{
    friend class Command;

    protected:
        /** Constructor with stringstream parameter. */
        CommandException(
                const stringstream &s /**< Message. */
                ): runtime_error(s.str()) {}
};

/****************************************************************************/

class Command
{
    public:
        Command(const string &, const string &);
        virtual ~Command();

        const string &getName() const;
        const string &getBriefDescription() const;

        typedef list<unsigned int> MasterIndexList;
        void setMasterIndices(const MasterIndexList &);
        const MasterIndexList &getMasterIndices() const;
        enum Verbosity {
            Quiet,
            Normal,
            Verbose
        };
        void setVerbosity(Verbosity);
        Verbosity getVerbosity() const;
        void setAlias(int);
        int getAlias() const;
        void setPosition(int);
        int getPosition() const;
        void setDomain(int);
        int getDomain() const;
        void setDataType(const string &);
        const string &getDataType() const;
        void setForce(bool);
        bool getForce() const;
        void setOutputFile(const string &);
        const string &getOutputFile() const;

        bool matchesSubstr(const string &) const;
        bool matchesAbbrev(const string &) const;

        virtual string helpString() const = 0;

        typedef vector<string> StringVector;
        virtual void execute(const StringVector &) = 0;

        static string numericInfo();

    protected:
        enum {BreakAfterBytes = 16};

        void throwInvalidUsageException(const stringstream &) const;
        void throwCommandException(const stringstream &) const;
        void throwSingleSlaveRequired(unsigned int) const;

        typedef list<ec_ioctl_slave_t> SlaveList;
        SlaveList selectedSlaves(MasterDevice &);
        typedef list<ec_ioctl_config_t> ConfigList;
        ConfigList selectedConfigs(MasterDevice &);
        typedef list<ec_ioctl_domain_t> DomainList;
        DomainList selectedDomains(MasterDevice &);

        static string alStateString(uint8_t);

    private:
        string name;
        string briefDesc;
        MasterIndexList masterIndices;
        Verbosity verbosity;
        int alias;
        int position;
        int domain;
        string dataType;
        bool force;
        string outputFile;

        Command();
};

/****************************************************************************/

inline const string &Command::getName() const
{
    return name;
}

/****************************************************************************/

inline const string &Command::getBriefDescription() const
{
    return briefDesc;
}

/****************************************************************************/

inline const Command::MasterIndexList &Command::getMasterIndices() const
{
    return masterIndices;
}

/****************************************************************************/

inline Command::Verbosity Command::getVerbosity() const
{
    return verbosity;
}

/****************************************************************************/

inline int Command::getAlias() const
{
    return alias;
}

/****************************************************************************/

inline int Command::getPosition() const
{
    return position;
}

/****************************************************************************/

inline int Command::getDomain() const
{
    return domain;
}

/****************************************************************************/

inline const string &Command::getDataType() const
{
    return dataType;
}

/****************************************************************************/

inline bool Command::getForce() const
{
    return force;
}

/****************************************************************************/

inline const string &Command::getOutputFile() const
{
    return outputFile;
}

/****************************************************************************/

#endif
