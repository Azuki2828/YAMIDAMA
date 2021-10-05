from Game import Normal
from Game import Move
from Game import Drink
from Game import GetYoiParam
from Game import ChangeState
from Game import LightCameraUpdate
from Game import FontUpdate

def Update() :

    Normal()
    Move()
    Drink()
    LightCameraUpdate()
    FontUpdate()

    yoiParam = GetYoiParam()

    if(yoiParam <= 50):
        ChangeState(0)

    elif(yoiParam < 100):
        ChangeState(1)

    else:
        ChangeState(2)
