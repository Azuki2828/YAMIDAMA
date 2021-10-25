from Game import ChangeState
from Game import Move
from Game import Rotate
from Game import GetCoolTime

def Update() :

    Move()
    Rotate()

    if(GetCoolTime() == 0.0):
        ChangeState(0)
