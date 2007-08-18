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

#include "Precompiled.hpp"

// -----------------------------------------------------------------------

#include "Modules/Module_Gan.hpp"
#include "Modules/Module_Obj.hpp"
#include "MachineBase/RLMachine.hpp"
#include "MachineBase/RLOperation.hpp"
#include "MachineBase/RLModule.hpp"
#include "MachineBase/LongOperation.hpp"
#include "Systems/Base/System.hpp"
#include "Systems/Base/GraphicsSystem.hpp"
#include "Systems/Base/GraphicsObject.hpp"
#include "Systems/Base/GanGraphicsObjectData.hpp"

#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;
using namespace libReallive;

// -----------------------------------------------------------------------

struct Gan_ganPlay : public RLOp_Void_2<IntConstant_T, IntConstant_T>
{
  struct WaitForGanToFinish : public LongOperation
  {
    int buf;
    WaitForGanToFinish(int in) : buf(in) {}

    bool operator()(RLMachine& machine)
    {
      GraphicsObject& obj = getGraphicsObject(machine, OBJ_FG_LAYER, buf);

      if(!obj.visible())
        cerr << "WARNING, OBJECT INVISIBLE!" << endl;

      if(obj.hasObjectData())
      {
        AnimatedObjectData* data = 
          dynamic_cast<AnimatedObjectData*>(&(obj.objectData()));
        if(data)
        {
          return !data->isPlaying();
        }
        else
          return true;
      }
      else
        return true;
    }
  };

  bool m_block;
  AnimatedObjectData::AfterAnimation m_afterEffect;

  Gan_ganPlay(bool block, 
              AnimatedObjectData::AfterAnimation after) 
    : m_block(block), m_afterEffect(after) {}

  void operator()(RLMachine& machine, int buf, int animationSet)
  {
    GraphicsObject& obj = getGraphicsObject(machine, OBJ_FG_LAYER, buf);

    if(obj.hasObjectData())
    {
      AnimatedObjectData* data = 
        dynamic_cast<AnimatedObjectData*>(&(obj.objectData()));
      if(data)
      {
        data->playSet(machine, animationSet);
        data->setAfterAction(m_afterEffect);

        if(m_block)
          machine.pushLongOperation(new WaitForGanToFinish(buf));
      }
    }
  }
};

// -----------------------------------------------------------------------

GanModule::GanModule()
  : RLModule("Gan", 1, 73)
{
  addOpcode(1003, 0, "ganPlay",
            new Gan_ganPlay(false, AnimatedObjectData::AFTER_NONE));
  addOpcode(1005, 0, "ganPlayOnce",
            new Gan_ganPlay(false, AnimatedObjectData::AFTER_CLEAR));
  addOpcode(1006, 0, "ganPlayEx",
            new Gan_ganPlay(true, AnimatedObjectData::AFTER_NONE));
  addOpcode(1007, 0, "ganPlayOnceEx",
            new Gan_ganPlay(true, AnimatedObjectData::AFTER_CLEAR));

  addOpcode(3003, 0, "ganPlay2", 
            new Gan_ganPlay(false, AnimatedObjectData::AFTER_NONE));
  addOpcode(3005, 0, "ganPlayOnce2",
            new Gan_ganPlay(false, AnimatedObjectData::AFTER_CLEAR));

  // Bg versions!?
}