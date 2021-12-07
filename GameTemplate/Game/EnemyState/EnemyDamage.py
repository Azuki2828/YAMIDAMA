from Game import ChangeState
from Game import GetCoolTime
from Game import AnimationUpdate
from Game import IsDeath
from Game import SetCoolTime
from Game import UpdateTriggerBox

def Update() :

    #if(IsDeath()):
    #     ChangeState(4)
    #     SetCoolTime(5.0)

     #クールタイムが0秒になったら待機状態に。
    if(GetCoolTime() == 0.0):
        ChangeState(0)
    

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()
