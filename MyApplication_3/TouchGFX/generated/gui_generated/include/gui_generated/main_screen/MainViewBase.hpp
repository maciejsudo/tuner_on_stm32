/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAINVIEWBASE_HPP
#define MAINVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/graph/GraphWrapAndClear.hpp>
#include <touchgfx/widgets/graph/GraphElements.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/graph/GraphLabels.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class MainViewBase : public touchgfx::View<MainPresenter>
{
public:
    MainViewBase();
    virtual ~MainViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::BoxWithBorder graphBackground;
    touchgfx::GraphWrapAndClear<128> graph;
    touchgfx::GraphElementLine graphLine1;
    touchgfx::PainterRGB565 graphLine1Painter;
    touchgfx::GraphElementGridX graphMajorXAxisGrid;
    touchgfx::GraphElementGridY graphMajorYAxisGrid;
    touchgfx::GraphLabelsY graphMajorYAxisLabel;
    touchgfx::TextArea textArea1;
    touchgfx::TextArea textArea2;
    touchgfx::TextAreaWithOneWildcard textArea3;
    touchgfx::TextArea textArea4;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTAREA3_SIZE = 20;
    touchgfx::Unicode::UnicodeChar textArea3Buffer[TEXTAREA3_SIZE];

private:

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 12000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // MAINVIEWBASE_HPP
