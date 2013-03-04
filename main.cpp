#include "browserapplication.h"

int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(data);
    BrowserApplication application(argc, argv);
    if (!application.isTheOnlyBrowser())
        return 0;
    application.newMainWindow();
    return application.exec();
}

