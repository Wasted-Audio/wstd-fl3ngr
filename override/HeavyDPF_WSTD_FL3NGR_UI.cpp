/**
 * Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later
 */

#include "DistrhoUI.hpp"
#include "ResizeHandle.hpp"
#include "veramobd.hpp"
#include "wstdcolors.hpp"


START_NAMESPACE_DISTRHO

// --------------------------------------------------------------------------------------------------------------------

class ImGuiPluginUI : public UI
{
    float fhigh = 0.0f;
    float fhigh_feedback = 0.0f;
    float fhigh_intensity = 20.0f;
    float fhigh_mix = 50.0f;
    float fhigh_speed = 2.0f;
    float flow = 0.0f;
    float flow_feedback = 0.0f;
    float flow_intensity = 20.0f;
    float flow_mix = 50.0f;
    float flow_speed = 2.0f;
    float fmid = 0.0f;
    float fmid_feedback = 0.0f;
    float fmid_freq = 1337.0f;
    float fmid_intensity = 20.0f;
    float fmid_mix = 50.0f;
    float fmid_speed = 2.0f;
    bool fhigh_range = false;
    bool flow_range = false;
    bool fmid_range = false;

    ResizeHandle fResizeHandle;

    // ----------------------------------------------------------------------------------------------------------------

public:
   /**
      UI class constructor.
      The UI should be initialized to a default state that matches the plugin side.
    */
    ImGuiPluginUI()
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true),
          fResizeHandle(this)
    {
        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);

        ImGuiIO& io(ImGui::GetIO());

        ImFontConfig fc;
        fc.FontDataOwnedByAtlas = true;
        fc.OversampleH = 1;
        fc.OversampleV = 1;
        fc.PixelSnapH = true;

        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 16.0f * getScaleFactor(), &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 21.0f * getScaleFactor(), &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 11.0f * getScaleFactor(), &fc);
        io.Fonts->Build();
        io.FontDefault = io.Fonts->Fonts[1];

        fResizeHandle.hide();
    }

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // DSP/Plugin Callbacks

   /**
      A parameter has changed on the plugin side.@n
      This is called by the host to inform the UI about parameter changes.
    */
    void parameterChanged(uint32_t index, float value) override
    {
        switch (index) {
            case 0:
                fhigh = value;
                break;
            case 1:
                fhigh_feedback = value;
                break;
            case 2:
                fhigh_intensity = value;
                break;
            case 3:
                fhigh_mix = value;
                break;
            case 4:
                fhigh_speed = value;
                break;
            case 5:
                flow = value;
                break;
            case 6:
                flow_feedback = value;
                break;
            case 7:
                flow_intensity = value;
                break;
            case 8:
                flow_mix = value;
                break;
            case 9:
                flow_speed = value;
                break;
            case 10:
                fmid = value;
                break;
            case 11:
                fmid_feedback = value;
                break;
            case 12:
                fmid_freq = value;
                break;
            case 13:
                fmid_intensity = value;
                break;
            case 14:
                fmid_mix = value;
                break;
            case 15:
                fmid_speed = value;
                break;

            default: return;
        }

        repaint();
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Widget Callbacks

   /**
      ImGui specific onDisplay function.
    */
    void onImGuiDisplay() override
    {
        // Setup
        const float width = getWidth();
        const float height = getHeight();
        const float margin = 0.0f;

        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        style.Colors[ImGuiCol_TitleBgActive] = (ImVec4)WstdTitleBgActive;
        style.Colors[ImGuiCol_WindowBg] = (ImVec4)WstdWindowBg;

        ImGuiIO& io(ImGui::GetIO());
        ImFont* defaultFont = ImGui::GetFont();
        ImFont* titleBarFont = io.Fonts->Fonts[2];
        ImFont* smallFont = io.Fonts->Fonts[3];

        // Colors
        auto HighColorActive     = ColorBright(Blue,   fhigh);
        auto HighColorHovered    = ColorBright(BlueBr, fhigh);
        auto MidColorActive      =    ColorMid(Blue,   Green,   Red,   fmid, fmid_freq);
        auto MidColorHovered     =    ColorMid(BlueBr, GreenBr, RedBr, fmid, fmid_freq);
        auto MidFreqColorActive  =    ColorMid(BlueBr, GreenDr, RedBr, fmid, fmid_freq);
        auto MidFreqColorHovered =    ColorMid(Blue,   Green,   Red,   fmid, fmid_freq);
        auto LowColorActive      = ColorBright(Red,    flow);
        auto LowColorHovered     = ColorBright(RedBr,  flow);

        auto HighRangeSw         = ColorBright(WhiteDr, fhigh, false);
        auto HighRangeAct        = ColorBright(BlueDr, fhigh);
        auto HighRangeActHv      = ColorBright(Blue, fhigh);

        auto MidRangeSw          = ColorBright(WhiteDr, fmid, false);
        auto MidRangeAct         = MidFreqColorActive;
        auto MidRangeActHv       = MidFreqColorHovered;

        auto LowRangeSw          = ColorBright(WhiteDr, flow, false);
        auto LowRangeAct         = ColorBright(RedDr, flow);
        auto LowRangeActHv       = ColorBright(Red, flow);

        auto HighMixActive       = ColorMix(HighColorActive,  Yellow,   fhigh, fhigh_mix);
        auto HighMixHovered      = ColorMix(HighColorHovered, YellowBr, fhigh, fhigh_mix);
        auto MidMixActive        = ColorMix(MidColorActive,   Yellow,   fmid,  fmid_mix);
        auto MidMixHovered       = ColorMix(MidColorHovered,  YellowBr, fmid,  fmid_mix);
        auto LowMixActive        = ColorMix(LowColorActive,   Yellow,   flow,  flow_mix);
        auto LowMixHovered       = ColorMix(LowColorHovered,  YellowBr, flow,  flow_mix);

        // Sizes
        auto scaleFactor         = getScaleFactor();
        const float hundred      = 100 * scaleFactor;
        const float seventy      = 70 * scaleFactor;
        const float knobWidth    = 85 * scaleFactor;
        const float toggleWidth  = 18 * scaleFactor;
        const float eqText       = 45 * scaleFactor;

        // Steps
        auto high_speedstep      = 1.0f;
        auto mid_speedstep       = 1.0f;
        auto low_speedstep       = 1.0f;
        auto percstep            = 1.0f;
        auto dbstep              = 0.1f;
        auto hzstep              = 20.0f;

        if (io.KeyShift)
        {
            high_speedstep = (fhigh_range) ? 0.01f : 0.001f;
            mid_speedstep = (fmid_range) ? 0.01f : 0.001f;
            low_speedstep = (flow_range) ? 0.01f : 0.001f;

            percstep = 0.1f;
            dbstep = 0.01f;
            hzstep = 1.0f;
        }
        else
        {
            high_speedstep = (fhigh_range) ? 0.1f : 0.01f;
            mid_speedstep = (fmid_range) ? 0.1f : 0.01f;
            low_speedstep = (flow_range) ? 0.1f : 0.01f;
        }

        if(fhigh_speed > 2.0f)
            fhigh_range = true;

        if(fmid_speed > 2.0f)
            fmid_range = true;

        if(flow_speed > 2.0f)
            flow_range = true;

        // Draw
        ImGui::PushFont(titleBarFont);
        if (ImGui::Begin("WSTD FL3NGR", nullptr, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoCollapse))
        {
            ImGui::Dummy(ImVec2(0.0f, 8.0f) * scaleFactor);
            ImGui::PushFont(defaultFont);
            auto ImGuiKnob_Flags = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick + ImGuiKnobFlags_NoTitle;
            auto ImGuiKnob_FlagsDB = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
            auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

            // EQ Text
            ImGui::BeginGroup();
            {
                ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
                ImGui::Dummy(ImVec2(0.0f, 38.0f) * scaleFactor);
                CenterTextX("High", eqText);
                ImGui::Dummy(ImVec2(0.0f, 80.0f) * scaleFactor);
                CenterTextX("Mid", eqText);
                ImGui::Dummy(ImVec2(0.0f, 60.0f) * scaleFactor);
                ImGui::PushFont(smallFont);
                CenterTextX("Mid", eqText);
                CenterTextX("Freq", eqText);
                ImGui::PushFont(defaultFont);
                ImGui::Dummy(ImVec2(0.0f, 60.0f) * scaleFactor);
                CenterTextX("Low", eqText);
                ImGui::PopStyleColor();
            }
            ImGui::EndGroup();
            ImGui::SameLine();

            // EQ Section
            ImGui::BeginGroup();
            {
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
                if (ImGuiKnobs::Knob("High", &fhigh, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
                {

                    if (ImGui::IsItemActivated())
                    {
                        editParameter(0, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            fhigh = 0.0f;
                    }
                    setParameterValue(0, fhigh);
                }
                ImGui::PopStyleColor(2);

                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidColorHovered);
                if (ImGuiKnobs::Knob("Mid", &fmid, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(10, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            fmid = 0.0f;
                    }
                    setParameterValue(10, fmid);
                }
                ImGui::PopStyleColor(2);

                ImGui::Dummy(ImVec2(7.5f, 0.0f) * scaleFactor); ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidFreqColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidFreqColorHovered);
                if (ImGuiKnobs::Knob("Mid Freq", &fmid_freq, 313.3f, 5705.6f, hzstep, "%.1fHz", ImGuiKnobVariant_SteppedTick, seventy, ImGuiKnob_FlagsLog, 11))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(12, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            fmid_freq = 1337.0f;
                    }
                    setParameterValue(12, fmid_freq);
                }
                ImGui::PopStyleColor(2);

                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowColorHovered);
                if (ImGuiKnobs::Knob("Low", &flow, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(5, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            flow = 0.0f;
                    }
                    setParameterValue(5, flow);
                }
                ImGui::PopStyleColor(2);
            }
            ImGui::EndGroup(); ImGui::SameLine();
            ImGui::Dummy(ImVec2(20.0f, 0.0f) * scaleFactor); ImGui::SameLine();

            // High Band
            ImGui::BeginGroup();
            {
                ImGui::BeginGroup();
                {
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
                    if (ImGuiKnobs::Knob("High Intensity", &fhigh_intensity, 0.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(2, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fhigh_intensity = 20.0f;
                        }
                        setParameterValue(2, fhigh_intensity);
                    }
                    ImGui::SameLine();

                    // High Speed knob and range
                    ImGui::BeginGroup();
                    {
                        auto fhigh_max = 20.0f;
                        auto fhigh_steps = 21;
                        if (not fhigh_range)
                        {
                            fhigh_max = 2.0f;
                            fhigh_steps = 11;
                        }

                        // High Speed knob
                        if (ImGuiKnobs::Knob("High Speed", &fhigh_speed, 0.0f, fhigh_max, high_speedstep, "%.3fHz", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, fhigh_steps))
                        {
                            if (ImGui::IsItemActivated())
                            {
                                editParameter(4, true);
                                if (ImGui::IsMouseDoubleClicked(0))
                                    fhigh_speed = 2.0f;
                            }
                            setParameterValue(4, fhigh_speed);
                        }
                        ImGui::SameLine();

                        // High Speed range
                        ImGui::BeginGroup();
                        {
                            ImGui::Dummy(ImVec2(0.0f, 20.0f) * scaleFactor);

                            // Range text
                            ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
                            ImGui::PushFont(smallFont);
                            auto rangedef = (fhigh_range) ? "fast": "slow";
                            CenterTextX(rangedef, toggleWidth);
                            ImGui::PushFont(defaultFont);
                            ImGui::PopStyleColor();

                            // knob
                            ImGui::PushStyleColor(ImGuiCol_Text,            (ImVec4)HighRangeSw);

                            // inactive colors
                            ImGui::PushStyleColor(ImGuiCol_FrameBg,         (ImVec4)HighRangeAct);
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,  (ImVec4)HighRangeActHv);

                            // active colors
                            ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)HighRangeAct);
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighRangeActHv);

                            if (ImGui::Toggle("##High_Range", &fhigh_range, ImGuiToggleFlags_Animated))
                            {
                                if (ImGui::IsItemActivated() && !fhigh_range)
                                {
                                    editParameter(4, true);
                                    fhigh_speed = std::min(fhigh_speed, 2.0f);
                                    setParameterValue(4, fhigh_speed);
                                }
                            }
                            ImGui::PopStyleColor(5);
                        }
                        ImGui::EndGroup();
                    }
                    ImGui::EndGroup();
                    ImGui::SameLine();
                    if (ImGuiKnobs::Knob("High Feedback", &fhigh_feedback, -100.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SpaceBipolar, hundred, ImGuiKnob_Flags))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(1, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fhigh_feedback = 0.0f;
                        }
                        setParameterValue(1, fhigh_feedback);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighMixActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighMixHovered);
                    if (ImGuiKnobs::Knob("High Mix", &fhigh_mix, 0.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(3, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fhigh_mix = 50.0f;
                        }
                        setParameterValue(3, fhigh_mix);
                    }
                    ImGui::PopStyleColor(2);
                }
                ImGui::EndGroup();

                // Mid Band
                ImGui::BeginGroup();
                {
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidColorHovered);
                    if (ImGuiKnobs::Knob("Mid Intensity", &fmid_intensity, 0.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(13, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fmid_intensity = 20.0f;
                        }
                        setParameterValue(13, fmid_intensity);
                    }
                    ImGui::SameLine();

                    // Mid Speed knob and range
                    ImGui::BeginGroup();
                    {
                        auto fmid_max = 20.0f;
                        auto fmid_steps = 21;
                        if (not fmid_range)
                        {
                            fmid_max = 2.0f;
                            fmid_steps = 11;
                        }

                        // Mid Speed knob
                        if (ImGuiKnobs::Knob("Mid Speed", &fmid_speed, 0.0f, fmid_max, mid_speedstep, "%.3fHz", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, fmid_steps))
                        {
                            if (ImGui::IsItemActivated())
                            {
                                editParameter(15, true);
                                if (ImGui::IsMouseDoubleClicked(0))
                                    fmid_speed = 2.0f;
                            }
                            setParameterValue(15, fmid_speed);
                        }
                        ImGui::SameLine();

                        // Mid Speed range
                        ImGui::BeginGroup();
                        {
                            ImGui::Dummy(ImVec2(0.0f, 20.0f) * scaleFactor);

                            // Range text
                            ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
                            ImGui::PushFont(smallFont);
                            auto rangedef = (fmid_range) ? "fast": "slow";
                            CenterTextX(rangedef, toggleWidth);
                            ImGui::PushFont(defaultFont);
                            ImGui::PopStyleColor();

                            // knob
                            ImGui::PushStyleColor(ImGuiCol_Text,            (ImVec4)MidRangeSw);

                            // inactive colors
                            ImGui::PushStyleColor(ImGuiCol_FrameBg,         (ImVec4)MidRangeAct);
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,  (ImVec4)MidRangeActHv);

                            // active colors
                            ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)MidRangeAct);
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidRangeActHv);

                            if (ImGui::Toggle("##Mid_Range", &fmid_range, ImGuiToggleFlags_Animated))
                            {
                                if (ImGui::IsItemActivated() && !fmid_range)
                                {
                                    editParameter(15, true);
                                    fmid_speed = std::min(fmid_speed, 2.0f);
                                    setParameterValue(15, fmid_speed);
                                }
                            }
                            ImGui::PopStyleColor(5);
                        }
                        ImGui::EndGroup();

                    }
                    ImGui::EndGroup();
                    ImGui::SameLine();

                    if (ImGuiKnobs::Knob("Mid Feedback", &fmid_feedback, -100.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SpaceBipolar, hundred, ImGuiKnob_Flags))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(11, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fmid_feedback = 0.0f;
                        }
                        setParameterValue(11, fmid_feedback);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidMixActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidMixHovered);
                    if (ImGuiKnobs::Knob("Mid Mix", &fmid_mix, 0.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(14, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fmid_mix = 50.0f;
                        }
                        setParameterValue(14, fmid_mix);
                    }
                    ImGui::PopStyleColor(2);
                }
                ImGui::EndGroup();

                // Effect Headers
                ImGui::Dummy(ImVec2(0.0f, 23.0f) * scaleFactor);
                ImGui::BeginGroup();
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
                    CenterTextX("Intensity", knobWidth); ImGui::SameLine();
                    CenterTextX("Speed", knobWidth); ImGui::SameLine();
                    CenterTextX("Range", toggleWidth); ImGui::SameLine();
                    CenterTextX("Feedback", knobWidth); ImGui::SameLine();
                    CenterTextX("Mix", knobWidth);
                    ImGui::PopStyleColor();
                }
                ImGui::EndGroup();
                ImGui::Dummy(ImVec2(0.0f, 23.0f) * scaleFactor);

                // Low Band
                ImGui::BeginGroup();
                {
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowColorHovered);
                    if (ImGuiKnobs::Knob("Low Intensity", &flow_intensity, 0.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(7, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                flow_intensity = 20.0f;
                        }
                        setParameterValue(7, flow_intensity);
                    }
                    ImGui::SameLine();

                    // Low Speed knob and range
                    ImGui::BeginGroup();
                    {
                        auto flow_max = 20.0f;
                        auto flow_steps = 21;
                        if (not flow_range)
                        {
                            flow_max = 2.0f;
                            flow_steps = 11;
                        }

                        // Low Speed knob
                        if (ImGuiKnobs::Knob("Low Speed", &flow_speed, 0.0f, flow_max, low_speedstep, "%.3fHz", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, flow_steps))
                        {
                            if (ImGui::IsItemActivated())
                            {
                                editParameter(9, true);
                                if (ImGui::IsMouseDoubleClicked(0))
                                    flow_speed = 2.0f;
                            }
                            setParameterValue(9, flow_speed);
                        }
                        ImGui::SameLine();

                        // Low Speed range
                        ImGui::BeginGroup();
                        {
                            ImGui::Dummy(ImVec2(0.0f, 20.0f) * scaleFactor);

                            // Range text
                            ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
                            ImGui::PushFont(smallFont);
                            auto rangedef = (flow_range) ? "fast": "slow";
                            CenterTextX(rangedef, toggleWidth);
                            ImGui::PushFont(defaultFont);
                            ImGui::PopStyleColor();

                            // knob
                            ImGui::PushStyleColor(ImGuiCol_Text,            (ImVec4)LowRangeSw);

                            // inactive colors
                            ImGui::PushStyleColor(ImGuiCol_FrameBg,         (ImVec4)LowRangeAct);
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,  (ImVec4)LowRangeActHv);

                            // active colors
                            ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)LowRangeAct);
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowRangeActHv);

                            if (ImGui::Toggle("##Low_Range", &flow_range, ImGuiToggleFlags_Animated))
                            {
                                if (ImGui::IsItemActivated() && !flow_range)
                                {
                                    editParameter(9, true);
                                    flow_speed = std::min(flow_speed, 2.0f);
                                    setParameterValue(9, flow_speed);
                                }
                            }
                            ImGui::PopStyleColor(5);
                        }
                        ImGui::EndGroup();
                    }
                    ImGui::EndGroup();
                    ImGui::SameLine();

                    if (ImGuiKnobs::Knob("Low Feedback", &flow_feedback, -100.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SpaceBipolar, hundred, ImGuiKnob_Flags))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(6, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                flow_feedback = 0.0f;
                        }
                        setParameterValue(6, flow_feedback);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowMixActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowMixHovered);
                    if (ImGuiKnobs::Knob("Low Mix", &flow_mix, 0.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(8, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                flow_mix = 50.0f;
                        }
                        setParameterValue(8, flow_mix);
                    }
                    ImGui::PopStyleColor(2);
                }
                ImGui::EndGroup();
            }
            ImGui::EndGroup();

            if (ImGui::IsItemDeactivated())
            {
                editParameter(0, false);
                editParameter(1, false);
                editParameter(2, false);
                editParameter(3, false);
                editParameter(4, false);
                editParameter(5, false);
                editParameter(6, false);
                editParameter(7, false);
                editParameter(8, false);
                editParameter(9, false);
                editParameter(10, false);
                editParameter(11, false);
                editParameter(12, false);
                editParameter(13, false);
                editParameter(14, false);
                editParameter(15, false);
            }

            ImGui::PopFont();
        }
        ImGui::PopFont();
        ImGui::End();
    }

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImGuiPluginUI)
};

// --------------------------------------------------------------------------------------------------------------------

UI* createUI()
{
    return new ImGuiPluginUI();
}

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
