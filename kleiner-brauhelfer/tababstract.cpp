#include "tababstract.h"

TabAbstract::TabAbstract(QWidget *parent) :
    QWidget(parent),
    mIsTabActive(false)
{
}

void TabAbstract::saveSettings()
{
}

void TabAbstract::restoreView(bool full)
{
    Q_UNUSED(full)
}

void TabAbstract::setTabActive(bool active)
{
    if (mIsTabActive != active)
    {
        mIsTabActive = active;
        if (mIsTabActive)
            onTabActivated();
        else
            onTabDeactivated();
    }
}

bool TabAbstract::isTabActive() const
{
    return mIsTabActive;
}

void TabAbstract::onTabActivated()
{
}

void TabAbstract::onTabDeactivated()
{
    setFocus();
}

void TabAbstract::printPreview()
{
}

void TabAbstract::toPdf()
{
}
