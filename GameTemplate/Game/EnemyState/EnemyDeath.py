from Game import GetCoolTime
from Game import AnimationUpdate
from Game import Delete

def Update() :


    #クールタイムが0秒になったら消える。
    if(GetCoolTime() == 0.0):
        #自信を削除。
        Delete()    

    #アニメーションを更新。
    AnimationUpdate()