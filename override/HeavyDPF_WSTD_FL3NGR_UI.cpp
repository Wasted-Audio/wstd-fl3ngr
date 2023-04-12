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

    ResizeHandle fResizeHandle;

    // ----------------------------------------------------------------------------------------------------------------

public:
   /**
      UI class constructor.
      The UI should be initialized to a default state that matches the plugin side.
    */
    ImGuiPluginUI()
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT),
          fResizeHandle(this)
    {
        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);

        ImGuiIO& io(ImGui::GetIO());

        ImFontConfig fc;
        fc.FontDataOwnedByAtlas = true;
        fc.OversampleH = 1;
        fc.OversampleV = 1;
        fc.PixelSnapH = true;

        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 16.0f, &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 21.0f, &fc);
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

        const float width = getWidth();
        const float height = getHeight();
        const float margin = 0.0f;

        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);


        style.Colors[ImGuiCol_TitleBgActive] = (ImVec4)ImColor::HSV(3.31f / 3.6f, 0.64f, 0.40f);
        style.Colors[ImGuiCol_WindowBg] = (ImVec4)ImColor::HSV(3.31f / 3.6f, 0.64f, 0.10f);

        ImGuiIO& io(ImGui::GetIO());
        ImFont* defaultFont = ImGui::GetFont();
        ImFont* titleBarFont = io.Fonts->Fonts[2];

        auto HighColorActive     = ColorBright(Blue,   fhigh);
        auto HighColorHovered    = ColorBright(BlueBr, fhigh);
        auto MidColorActive      =    ColorMid(Blue,   Green,   Red,   fmid, fmid_freq);
        auto MidColorHovered     =    ColorMid(BlueBr, GreenBr, RedBr, fmid, fmid_freq);
        auto MidFreqColorActive  =    ColorMid(BlueBr, GreenDr, RedBr, fmid, fmid_freq);
        auto MidFreqColorHovered =    ColorMid(Blue,   Green,   Red,   fmid, fmid_freq);
        auto LowColorActive      = ColorBright(Red,    flow);
        auto LowColorHovered     = ColorBright(RedBr,  flow);
        
        auto HighMixActive       = ColorMix(HighColorActive,  Yellow,   fhigh, fhigh_mix);
        auto HighMixHovered      = ColorMix(HighColorHovered, YellowBr, fhigh, fhigh_mix);
        auto MidMixActive        = ColorMix(MidColorActive,   Yellow,   fmid,  fmid_mix);
        auto MidMixHovered       = ColorMix(MidColorHovered,  YellowBr, fmid,  fmid_mix);
        auto LowMixActive        = ColorMix(LowColorActive,   Yellow,   flow,  flow_mix);
        auto LowMixHovered       = ColorMix(LowColorHovered,  YellowBr, flow,  flow_mix);

        ImGui::PushFont(titleBarFont);
        if (ImGui::Begin("WSTD FL3NGR", nullptr, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoCollapse))
        {

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::PushFont(defaultFont);
            auto ImGuiKnob_Flags = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick + ImGuiKnobFlags_NoTitle;
            auto ImGuiKnob_FlagsDB = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
            auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

            ImGui::BeginGroup();
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.85f, 0.85f));
                ImGui::Dummy(ImVec2(0.0f, 38.0f));
                ImGui::Dummy(ImVec2(14.0f, 0.0f)); ImGui::SameLine();
                ImGui::Text("High");
                ImGui::Dummy(ImVec2(0.0f, 80.0f));
                ImGui::Dummy(ImVec2(17.0f, 0.0f)); ImGui::SameLine();
                ImGui::Text("Mid");
                ImGui::Dummy(ImVec2(0.0f, 60.0f));
                ImGui::Dummy(ImVec2(17.0f, 0.0f)); ImGui::SameLine();
                ImGui::Text("Mid");
                ImGui::Dummy(ImVec2(14.0f, 0.0f)); ImGui::SameLine();
                ImGui::Text("Freq");
                ImGui::Dummy(ImVec2(0.0f, 55.0f));
                ImGui::Dummy(ImVec2(17.0f, 0.0f)); ImGui::SameLine();
                ImGui::Text("Low");
                ImGui::PopStyleColor();
            }
            ImGui::EndGroup();
            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
                if (ImGuiKnobs::Knob("High", &fhigh, -15.0f, 15.0, 0.2f, "%.1fdB", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_FlagsDB, 7))
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
                if (ImGuiKnobs::Knob("Mid", &fmid, -15.0f, 15.0, 0.2f, "%.1fdB", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_FlagsDB, 7))
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

                ImGui::SetCursorPosX(85.0f);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidFreqColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidFreqColorHovered);
                if (ImGuiKnobs::Knob("Mid Freq", &fmid_freq, 313.3f, 5705.6f, 50.0f, "%.1fHz", ImGuiKnobVariant_SteppedTick, 70, ImGuiKnob_FlagsLog, 11))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(11, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            fmid_freq = 1337.0f;
                    }
                    setParameterValue(11, fmid_freq);
                }
                ImGui::PopStyleColor(2);

                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowColorHovered);
                if (ImGuiKnobs::Knob("Low", &flow, -15.0f, 15.0, 0.2f, "%.1fdB", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_FlagsDB, 7))
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
            ImGui::Dummy(ImVec2(20.0f, 0.0f)); ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ImGui::BeginGroup();
                {
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
                    if (ImGuiKnobs::Knob("High Intensity", &fhigh_intensity, 0.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(2, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fhigh_intensity = 20.0f;

                        }
                        setParameterValue(2, fhigh_intensity);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
                    if (ImGuiKnobs::Knob("High Speed", &fhigh_speed, 0.0f, 20.0f, 0.05f, "%.1fHz", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 21))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(4, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fhigh_speed = 2.0f;

                        }
                        setParameterValue(4, fhigh_speed);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
                    if (ImGuiKnobs::Knob("High Feedback", &fhigh_feedback, -100.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SpaceBipolar, 100, ImGuiKnob_Flags))
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
                    if (ImGuiKnobs::Knob("High Mix", &fhigh_mix, 0.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 11))
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

                ImGui::BeginGroup();
                {
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidColorHovered);
                    if (ImGuiKnobs::Knob("Mid Intensity", &fmid_intensity, 0.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(13, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fmid_intensity = 20.0f;

                        }
                        setParameterValue(13, fmid_intensity);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidColorHovered);
                    if (ImGuiKnobs::Knob("Mid Speed", &fmid_speed, 0.0f, 20.0f, 0.05f, "%.1fHz", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 21))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(15, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                fmid_speed = 2.0f;

                        }
                        setParameterValue(15, fmid_speed);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidColorHovered);
                    if (ImGuiKnobs::Knob("Mid Feedback", &fmid_feedback, -100.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SpaceBipolar, 100, ImGuiKnob_Flags))
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
                    if (ImGuiKnobs::Knob("Mid Mix", &fmid_mix, 0.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 11))
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

                ImGui::Dummy(ImVec2(0.0f, 23.0f));
                ImGui::BeginGroup();
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.85f, 0.85f));
                    ImGui::Dummy(ImVec2(5.0f, 0.0f)); ImGui::SameLine();
                    ImGui::Text("Intensity"); ImGui::SameLine();
                    ImGui::Dummy(ImVec2(35.0f, 0.0f)); ImGui::SameLine();
                    ImGui::Text("Speed"); ImGui::SameLine();
                    ImGui::Dummy(ImVec2(45.0f, 0.0f)); ImGui::SameLine();
                    ImGui::Text("Feedback"); ImGui::SameLine();
                    ImGui::Dummy(ImVec2(43.0f, 0.0f)); ImGui::SameLine();
                    ImGui::Text("Mix"); ImGui::SameLine();
                    ImGui::PopStyleColor();
                }
                ImGui::EndGroup();
                ImGui::Dummy(ImVec2(0.0f, 23.0f));

                ImGui::BeginGroup();
                {
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowColorHovered);
                    if (ImGuiKnobs::Knob("Low Intensity", &flow_intensity, 0.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 11))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(7, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                flow_intensity = 20.0f;

                        }
                        setParameterValue(7, flow_intensity);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowColorHovered);
                    if (ImGuiKnobs::Knob("Low Speed", &flow_speed, 0.0f, 20.0f, 0.05f, "%.1fHz", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 21))
                    {
                        if (ImGui::IsItemActivated())
                        {
                            editParameter(9, true);
                            if (ImGui::IsMouseDoubleClicked(0))
                                flow_speed = 2.0f;

                        }
                        setParameterValue(9, flow_speed);
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowColorActive);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowColorHovered);
                    if (ImGuiKnobs::Knob("Low Feedback", &flow_feedback, -100.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SpaceBipolar, 100, ImGuiKnob_Flags))
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
                    if (ImGuiKnobs::Knob("Low Mix", &flow_mix, 0.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 11))
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
