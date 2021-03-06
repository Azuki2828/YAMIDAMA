from Game import Move
from Game import Rotate
from Game import ChangeState
from Game import GetLengthToPlayer
from Game import SetCoolTime
from Game import AnimationUpdate
from Game import UpdateTriggerBox
from Game import JudgeDamage
import random
import time

def Update():

    #移動関数。
    Move()

    #回転関数。
    Rotate()

    #ダメージ判定
    JudgeDamage()

    #プレイヤーとの距離を調べる。
    lengthToPlayer = GetLengthToPlayer()

    #乱数を生成
    carentTime = time.time()
    random.seed(carentTime)
    attack = random.randrange(4)
    
    #プレイヤーとの距離が120以下なら
    if(lengthToPlayer <= 120.0):
    #判定がバックならバック状態に設定。
        if(attack == 0):
           ChangeState(6)

           #クールタイムをランダムに設定。
           random.seed(carentTime + 1.0)
           coolTime = random.uniform(1, 2)
           SetCoolTime(coolTime)
    #そうじゃないなら攻撃状態に設定。
        else:
            ChangeState(2)

            #クールタイムを1.6秒に設定。
            SetCoolTime(1.6)
            
    #プレイヤーとの距離が500より遠いなら待機状態に。
    elif(lengthToPlayer > 500.0):
        ChangeState(0)

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()
