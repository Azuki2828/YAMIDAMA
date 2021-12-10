echo on

pushd
cd ../
rmdir /S /Q submission
mkdir submission
copy .\x64\master\Game.lib .\submission\Game.lib
copy .\x64\master\Game.pyd .\submission\Game.pyd
copy .\x64\master\GameBoot.exe .\submission\GameBoot.exe
copy .\Game\python37.dll .\submission\python37.dll
xcopy /e .\Game\Assets .\submission\Assets\
xcopy /e .\Game\EnemyState .\submission\EnemyState\
xcopy /e .\Game\Python37_64 .\submission\Python37_64\

pause
