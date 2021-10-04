from Game import Death
from Game import Move
from Game import Drink
from Game import GetYoiParam
from Game import ChangeState

def Update() :

    Death()
    Move()
    Drink()

    yoiParam = GetYoiParam()

    if(yoiParam <= 50):
        ChangeState(0)

    elif(yoiParam < 100):
        ChangeState(1)

    else:
        ChangeState(2)
