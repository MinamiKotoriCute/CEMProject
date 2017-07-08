@echo off
setlocal ENABLEDELAYEDEXPANSION
set /p name=¦WºÙ¡G

:: to small
set name_s=%name%
for %%a in (a b c d e f g h i j k l m n o p q r s t u v w x y z)do set "name_s=!name_s:%%a=%%a!"

:: to big
set name_b=%name%
for %%a in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z)do set "name_b=!name_b:%%a=%%a!"

mkdir %name%
cd %name%

::create %name_s%.pri
echo PROHECT_PATH = $$PWD>> %name_s%.pri
echo.>> %name_s%.pri
echo INCLUDEPATH += $${PROHECT_PATH}>> %name_s%.pri
echo.>> %name_s%.pri
echo SOURCES += $${PROHECT_PATH}\%name_s%.cpp>> %name_s%.pri
echo.>> %name_s%.pri
echo HEADERS  += $${PROHECT_PATH}\%name_s%.h>>%name_s%.pri

::create %name_s%.h
echo #ifndef %name_b%_H>>%name_s%.h
echo #define %name_b%_H>>%name_s%.h
echo.>> %name_s%.h
echo #include ^<QObject^>>>%name_s%.h
echo.>> %name_s%.h
echo class %name% : public QObject>>%name_s%.h
echo {>>%name_s%.h
echo     Q_OBJECT>>%name_s%.h
echo public:>>%name_s%.h
echo     explicit %name%(QObject *parent = 0);>>%name_s%.h
echo.>> %name_s%.h
echo signals:>>%name_s%.h
echo.>> %name_s%.h
echo public slots:>>%name_s%.h
echo };>>%name_s%.h
echo.>> %name_s%.h
echo #endif // %name_b%_H>>%name_s%.h

::create %name_s%.cpp
echo #include "%name_s%.h">>%name_s%.cpp
echo.>> %name_s%.cpp
echo %name%::%name%(QObject *parent)>>%name_s%.cpp
echo     : QObject(parent)>>%name_s%.cpp
echo {>>%name_s%.cpp
echo.>> %name_s%.cpp
echo }>>%name_s%.cpp

pause