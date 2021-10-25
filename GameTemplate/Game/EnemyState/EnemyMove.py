from Game import Move
from Game import Rotate
from Game import ChangeState
from Game import GetLengthToPlayer
from Game import SetCoolTime

def Update():

    Move()
    Rotate()

    lengthToPlayer = GetLengthToPlayer()

    if(lengthToPlayer < 50.0):
        ChangeState(2)
        SetCoolTime(3.0)
