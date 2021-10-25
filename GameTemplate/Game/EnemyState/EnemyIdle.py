from Game import ChangeState
from Game import GetLengthToPlayer
from Game import Move
from Game import Rotate
#from Game import Enemy


def Update() :

    Move()
    Rotate()

    lengthToPlayer = GetLengthToPlayer()

    if(lengthToPlayer < 500.0):
        ChangeState(1)