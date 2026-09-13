/**
 * @file alpanelquicksettings.cpp
 * @brief Base panel for quick settings popdown and floater
 *
 * $LicenseInfo:firstyear=2013&license=viewerlgpl$
 * Alchemy Viewer Source Code
 * Copyright (C) 2013-2014, Alchemy Viewer Project.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * $/LicenseInfo$
 */

#include "llviewerprecompiledheaders.h"

#include "alpanelquicksettings.h"

#include "llbutton.h"
#include "llcheckboxctrl.h"
#include "llcombobox.h"
#include "llenvironment.h"
#include "llfeaturemanager.h"
#include "llslider.h"
#include "llspinctrl.h"

#include "llagent.h"
#include "llviewercontrol.h"
#include "llviewerregion.h"
#include "llvoavatar.h"
#include "llvoavatarself.h"
#include "rlvactions.h"

static LLPanelInjector<ALPanelQuickSettings> t_quick_settings("quick_settings");

ALPanelQuickSettings::ALPanelQuickSettings()
    : LLPanel(),
    mHoverSlider(nullptr),
    mHoverSpinner(nullptr)
{
}

ALPanelQuickSettings::~ALPanelQuickSettings()
{
    if (mRegionChangedSlot.connected())
    {
        mRegionChangedSlot.disconnect();
    }
}

// virtual
bool ALPanelQuickSettings::postBuild()
{
    refresh();

    // Fitzroy's three practical profiles intentionally map to Alchemy's
    // maintained feature levels. This keeps PBR enabled while allowing the
    // feature table and GPU masks to make safe hardware-specific decisions.
    getChild<LLButton>("profile_light_btn")->setCommitCallback(
        boost::bind(&ALPanelQuickSettings::applyGraphicsProfile, this, 1));
    getChild<LLButton>("profile_balanced_btn")->setCommitCallback(
        boost::bind(&ALPanelQuickSettings::applyGraphicsProfile, this, 3));
    getChild<LLButton>("profile_pbr_btn")->setCommitCallback(
        boost::bind(&ALPanelQuickSettings::applyGraphicsProfile, this, 6));

    getChild<LLButton>("environment_shared_btn")->setCommitCallback(
        boost::bind(&ALPanelQuickSettings::applyEnvironmentPreset, this, "shared"));
    getChild<LLButton>("environment_midday_btn")->setCommitCallback(
        boost::bind(&ALPanelQuickSettings::applyEnvironmentPreset, this, "midday"));
    getChild<LLButton>("environment_sunset_btn")->setCommitCallback(
        boost::bind(&ALPanelQuickSettings::applyEnvironmentPreset, this, "sunset"));
    getChild<LLButton>("environment_midnight_btn")->setCommitCallback(
        boost::bind(&ALPanelQuickSettings::applyEnvironmentPreset, this, "midnight"));

    // Hover height
    mHoverSlider = getChild<LLSlider>("hover_slider_bar");
    mHoverSlider->setMinValue(MIN_HOVER_Z);
    mHoverSlider->setMaxValue(MAX_HOVER_Z);
    mHoverSlider->setMouseUpCallback(boost::bind(&ALPanelQuickSettings::onHoverSliderFinalCommit, this));
    mHoverSlider->setCommitCallback(boost::bind(&ALPanelQuickSettings::onHoverSliderMoved, this, _2));

    mHoverSpinner = getChild<LLSpinCtrl>("hover_spinner");
    mHoverSpinner->setMinValue(MIN_HOVER_Z);
    mHoverSpinner->setMaxValue(MAX_HOVER_Z);

    // Initialize slider from pref setting.
    syncFromPreferenceSetting();

    // Update slider on future pref changes.
    gSavedPerAccountSettings.getControl("AvatarHoverOffsetZ")->getCommitSignal()->connect(boost::bind(&ALPanelQuickSettings::syncFromPreferenceSetting, this));

    updateEditHoverEnabled();

    if (!mRegionChangedSlot.connected())
    {
        mRegionChangedSlot = gAgent.addRegionChangedCallback(boost::bind(&ALPanelQuickSettings::onRegionChanged, this));
    }
    // Set up based on initial region.
    onRegionChanged();

    return LLPanel::postBuild();
}

void ALPanelQuickSettings::applyGraphicsProfile(U32 level)
{
    LLFeatureManager::getInstance()->setGraphicsLevel(level, false);
    gSavedSettings.setU32("RenderQualityPerformance", level);
}

void ALPanelQuickSettings::applyEnvironmentPreset(const std::string& preset)
{
    if (!RlvActions::canChangeEnvironment())
    {
        return;
    }

    LLEnvironment& environment = LLEnvironment::instance();
    if (preset == "shared")
    {
        environment.setSharedEnvironment();
        return;
    }

    LLUUID sky_id;
    if (preset == "midday")
    {
        sky_id = LLEnvironment::KNOWN_SKY_MIDDAY;
    }
    else if (preset == "sunset")
    {
        sky_id = LLEnvironment::KNOWN_SKY_SUNSET;
    }
    else if (preset == "midnight")
    {
        sky_id = LLEnvironment::KNOWN_SKY_MIDNIGHT;
    }

    if (!sky_id.isNull())
    {
        environment.setEnvironment(LLEnvironment::ENV_LOCAL, sky_id, LLEnvironment::TRANSITION_FAST);
        environment.setSelectedEnvironment(LLEnvironment::ENV_LOCAL, LLEnvironment::TRANSITION_FAST);
    }
}

// virtual
void ALPanelQuickSettings::refresh()
{
    LLPanel::refresh();
}

void ALPanelQuickSettings::syncFromPreferenceSetting()
{
    F32 value = gSavedPerAccountSettings.getF32("AvatarHoverOffsetZ");
    mHoverSlider->setValue(value, false);
    mHoverSpinner->setValue(value);

    if (isAgentAvatarValid())
    {
        LLVector3 offset(0.0, 0.0, llclamp(value, MIN_HOVER_Z, MAX_HOVER_Z));
        LL_INFOS("Avatar") << "setting hover from preference setting " << offset[2] << LL_ENDL;
        gAgentAvatarp->setHoverOffset(offset);
    }
}

void ALPanelQuickSettings::onHoverSliderMoved(const LLSD& val)
{
    if (isAgentAvatarValid())
    {
        auto value = static_cast<F32>(val.asReal());
        LLVector3 offset(0.0, 0.0, llclamp(value, MIN_HOVER_Z, MAX_HOVER_Z));
        LL_INFOS("Avatar") << "setting hover from slider moved" << offset[2] << LL_ENDL;
        gAgentAvatarp->setHoverOffset(offset, false);
    }
}

// Do send-to-the-server work when slider drag completes, or new
// value entered as text.
void ALPanelQuickSettings::onHoverSliderFinalCommit()
{
    F32 value = mHoverSlider->getValueF32();
    gSavedPerAccountSettings.setF32("AvatarHoverOffsetZ", value);
    if (isAgentAvatarValid())
    {
        LLVector3 offset(0.0, 0.0, llclamp(value, MIN_HOVER_Z, MAX_HOVER_Z));
        LL_INFOS("Avatar") << "setting hover from slider final commit " << offset[2] << LL_ENDL;
        gAgentAvatarp->setHoverOffset(offset, true); // will send update this time.
    }
}

void ALPanelQuickSettings::onRegionChanged()
{
    LLViewerRegion *region = gAgent.getRegion();
    if (region && region->simulatorFeaturesReceived())
    {
        updateEditHoverEnabled();
    }
    else if (region)
    {
        region->setSimulatorFeaturesReceivedCallback(boost::bind(&ALPanelQuickSettings::onSimulatorFeaturesReceived, this, _1));
    }
}

void ALPanelQuickSettings::onSimulatorFeaturesReceived(const LLUUID &region_id)
{
    LLViewerRegion *region = gAgent.getRegion();
    if (region && (region->getRegionID() == region_id))
    {
        updateEditHoverEnabled();
    }
}

void ALPanelQuickSettings::updateEditHoverEnabled()
{
    bool enabled = gAgent.getRegion() && gAgent.getRegion()->avatarHoverHeightEnabled();
    mHoverSlider->setEnabled(enabled);
    mHoverSpinner->setEnabled(enabled);
    if (enabled)
    {
        syncFromPreferenceSetting();
    }
}
