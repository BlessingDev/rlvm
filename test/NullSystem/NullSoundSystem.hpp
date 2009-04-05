// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:sts=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2008 Elliot Glaysher
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
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

#ifndef __NullSoundSystem_hpp__
#define __NullSoundSystem_hpp__

#include "Systems/Base/SoundSystem.hpp"
#include "NullSystem/MockLog.hpp"
#include <string>

class Gameexe;

/**
 * Null sound system
 */
class NullSoundSystem : public SoundSystem
{
public:
  NullSoundSystem(System& system);
  ~NullSoundSystem();

  virtual int bgmStatus() const;

  virtual void bgmPlay(RLMachine& machine, const std::string& bgm_name, bool loop);
  virtual void bgmPlay(RLMachine& machine, const std::string& bgm_name, bool loop,
                       int fade_in_ms);
  virtual void bgmPlay(RLMachine& machine, const std::string& bgm_name, bool loop,
                       int fade_in_ms, int fade_out_ms);
  virtual void bgmStop();
  virtual void bgmPause();
  virtual void bgmUnPause();
  virtual void bgmFadeOut(int fade_out_ms);
  virtual std::string bgmName() const;
  virtual bool bgmLooping() const;

  virtual void wavPlay(RLMachine& machine, const std::string& wav_file,
                       bool loop);
  virtual void wavPlay(RLMachine& machine, const std::string& wav_file,
                       bool loop, const int channel);
  virtual void wavPlay(RLMachine& machine, const std::string& wav_file,
                       bool loop, const int channel, const int fadein_ms);
  virtual bool wavPlaying(RLMachine& machine, const int channel);
  virtual void wavStop(const int channel);
  virtual void wavStopAll();
  virtual void wavFadeOut(const int channel, const int fadetime);

  virtual void playSe(RLMachine& machine, const int se_num);

private:

  virtual void koePlayImpl(RLMachine& machine, int id);

  /// Record all method calls here
  mutable MockLog sound_system_log_;

  std::string bgm_name_;
};  // end of class NullSoundSystem


#endif
