#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>

//#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();


    virtual void updatewildcard(float newval);

protected:
    int tickCounter;

    void handleTickEvent();
    //virtual void sliderValueChanged(int value);
};

#endif // MAINVIEW_HPP
