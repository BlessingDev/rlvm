// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:sts=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2007 Elliot Glaysher
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//  
// -----------------------------------------------------------------------

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/scoped_ptr.hpp>

#include "Modules/Module_Str.hpp"
#include "libReallive/archive.h"
#include "libReallive/intmemref.h"
#include "MachineBase/RLMachine.hpp"
#include "MachineBase/Serialization.hpp"
#include "MachineBase/Memory.hpp"

#include "NullSystem/NullSystem.hpp"
#include "NullSystem/NullGraphicsSystem.hpp"
#include "Systems/Base/GraphicsObject.hpp"
#include "Systems/Base/GraphicsObjectOfFile.hpp"

#include "Utilities.h"
#include "testUtils.hpp"
#include "tut.hpp"
#include <boost/scoped_ptr.hpp>
#include <iostream>

using namespace boost;
using namespace std;
using namespace libReallive;
using namespace Serialization;

// -----------------------------------------------------------------------

namespace tut
{

const string FILE_NAME = "doesntmatter";

struct GraphicsSystem_data
{
  // Use any old test case; it isn't getting executed
  libReallive::Archive arc;
  NullSystem system;
  RLMachine rlmachine;

  GraphicsSystem_data()
	: arc(locateTestCase("Module_Str_SEEN/strcpy_0.TXT")),
	  system(),
	  rlmachine(system, arc)
  {}
};

typedef test_group<GraphicsSystem_data> tf;
typedef tf::object object;
tf GraphicsSystem_data("GraphicsSystem");

// -----------------------------------------------------------------------

/** 
 * Test the serialization of an individual GraphicsObjectOfFile
 * object.
 */
template<>
template<>
void object::test<1>()
{
  stringstream ss;
  Serialization::g_currentMachine = &rlmachine;

  {
    const scoped_ptr<GraphicsObjectData> inputObjOfFile(
      new GraphicsObjectOfFile(rlmachine, FILE_NAME));
    boost::archive::text_oarchive oa(ss);
    oa << inputObjOfFile;
  }

  {
    scoped_ptr<GraphicsObjectData> dst;
    boost::archive::text_iarchive ia(ss);
    ia >> dst;

    GraphicsObjectOfFile& obj = dynamic_cast<GraphicsObjectOfFile&>(*dst);
    ensure_equals("Preserved file name", obj.filename(), FILE_NAME);
  }

  Serialization::g_currentMachine = NULL;
}

// -----------------------------------------------------------------------

/**
 * Try it again, this time wrapped in the GraphicsObject
 */
template<>
template<>
void object::test<2>()
{
  stringstream ss;
  Serialization::g_currentMachine = &rlmachine;
  
  {
    const scoped_ptr<GraphicsObject> obj(new GraphicsObject());
    obj->setObjectData(new GraphicsObjectOfFile(rlmachine, FILE_NAME));

    boost::archive::text_oarchive oa(ss);
    oa << obj;
  }

  {
    scoped_ptr<GraphicsObject> dst;
    boost::archive::text_iarchive ia(ss);
    ia >> dst;

    GraphicsObjectOfFile& obj = 
      dynamic_cast<GraphicsObjectOfFile&>(dst->objectData());

    // Now query invariants.
    ensure_equals("Didn't preserve filename", obj.filename(), FILE_NAME);
  }

  Serialization::g_currentMachine = NULL;
}

// -----------------------------------------------------------------------

/**
 * Serialize the entire graphics system. ;_; I'm getting desperate on
 * this bug here...
 */
template<>
template<>
void object::test<3>()
{
  stringstream ss;
  Serialization::g_currentMachine = &rlmachine;
  
  {
    GraphicsObject obj;
    obj.setObjectData(new GraphicsObjectOfFile(rlmachine, FILE_NAME));
    system.graphics().setObject(OBJ_FG_LAYER, 1, obj);

    boost::archive::text_oarchive oa(ss);
    oa << const_cast<const GraphicsSystem&>(system.graphics());
  }

  {
    NullSystem sys;
    boost::archive::text_iarchive ia(ss);
    ia >> sys.graphics();

    GraphicsObjectOfFile& obj = 
      dynamic_cast<GraphicsObjectOfFile&>(
        sys.graphics().getObject(OBJ_FG_LAYER, 1).objectData());

    // Now query invariants.
    ensure_equals("Didn't preserve filename", obj.filename(), FILE_NAME);
  }

  Serialization::g_currentMachine = NULL;
}

// -----------------------------------------------------------------------

/**
 * 
 */
template<>
template<>
void object::test<4>()
{
  // Try serializing the whole damn thing.
  stringstream ss;
  GraphicsObject obj;
  obj.setObjectData(new GraphicsObjectOfFile(rlmachine, FILE_NAME));
  system.graphics().setObject(OBJ_FG_LAYER, 1, obj);
  saveGameTo(ss, rlmachine);

  {
    NullSystem otherSys;
    RLMachine omachine(otherSys, arc);
    loadGameFrom(ss, omachine);

    GraphicsObjectOfFile& obj = 
      dynamic_cast<GraphicsObjectOfFile&>(
        omachine.system().graphics().getObject(OBJ_FG_LAYER, 1).objectData());

    // Now query invariants.
    ensure_equals("Didn't preserve filename", obj.filename(), FILE_NAME);
  }
}

// -----------------------------------------------------------------------

};
