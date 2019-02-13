import json
import maya.cmds as cmds
import math

def matrix_to_quat(m):
    w = math.sqrt(1.0 + m[0] + m[5] + m[10]) / 2.0
    w4 = w * 4.0
    x = (m[6] - m[9]) / w4
    y = (m[8] - m[2]) / w4
    z = (m[1] - m[4]) / w4
    return [w, x, y, z]

jsonData = {}  
jsonData['GameObjects'] = []  

selected_obj = cmds.ls(sl=True, type='transform')
for curr_obj in selected_obj:
    translation = cmds.xform(curr_obj, query=True, translation=True, worldSpace=True)
    transformationMatrix = cmds.xform(curr_obj, query=True, matrix=True)
    orientation = matrix_to_quat(transformationMatrix)
    gameObjectModel = ""
    gameObjectType = ""
    gameObjectUniformMovementSpeed = 0.35
    # Movement Component
    gameObjectAcceleration = [0, 0, 0]
    gameObjectVelocity = [0, 0, 0]
    # Health Component
    gameObjectHealth = 100
    gameObjectMaxHealth = 100
    # Attack Component
    gameObjectAttackDamage = 20
    gameObjectAttackDistance = 30
    gameObjectAttackSpeed = 15

    if curr_obj.find("duck") >= 0:
        gameObjectType = "PlayerCharacter"
        gameObjectModel = "duck"
        gameObjectHealth = 100
        gameObjectMaxHealth = 100
    elif curr_obj.find("floor") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "floor"
    elif curr_obj.find("grass") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "grass"
    elif curr_obj.find("bush") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "bush"
    elif curr_obj.find("tree") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "tree"
    elif curr_obj.find("rock") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "rock"
    elif curr_obj.find("daisyFlowers") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "daisyFlowers"
    elif curr_obj.find("pebbles") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "pebbles"
    elif curr_obj.find("rock2") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "rock2"
    elif curr_obj.find("damagedTower") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "damagedTower"
    elif curr_obj.find("yellowFlowers") >= 0:
        gameObjectType = "StaticEnvironmentObject"
        gameObjectModel = "yellowFlowers"
    elif curr_obj.find("warTower") >= 0:
        gameObjectType = "EnemyTower"
        gameObjectModel = "warTower"
        gameObjectHealth = 500
        gameObjectMaxHealth = 500
        gameObjectAttackDamage = 20
        gameObjectAttackDistance = 30
        gameObjectAttackSpeed = 15
    elif curr_obj.find("flag") >= 0:
        gameObjectType = "EndLevelCollectable"
        gameObjectModel = "flag"
    elif curr_obj.find("minotaur") >= 0:
        gameObjectType = "DynamicEnvironmentObject"
        gameObjectModel = "minotaur"
        gameObjectAcceleration = [0.5, 0, 0]
        gameObjectVelocity = [-0.15, 0, 0]
    elif curr_obj.find("planet") >= 0:
        gameObjectType = "DynamicEnvironmentObject"
        gameObjectModel = "planet"
        gameObjectAcceleration = [0.75, 0.01, 0]
        gameObjectVelocity = [0.55, 0.002, 0]

    if gameObjectType != "":
        if gameObjectType == "PlayerCharacter":
            jsonData['GameObjects'].append({
                'modelName': gameObjectModel,
                'type': gameObjectType,
                'position': [translation[0], translation[1], translation[2]],
                'orientation': [orientation[0], orientation[1], orientation[2], orientation[3]],
                'scale': [1, 1, 1],
                'health': gameObjectHealth,
                'maxHealth': gameObjectMaxHealth,
                'uniformMovementSpeed': 0.35
            })
        elif gameObjectType == "StaticEnvironmentObject":
            jsonData['GameObjects'].append({
                'modelName': gameObjectModel,
                'type': gameObjectType,
                'position': [translation[0], translation[1], translation[2]],
                'orientation': [orientation[0], orientation[1], orientation[2], orientation[3]],
                'scale': [1, 1, 1]
            })
        elif gameObjectType == "DynamicEnvironmentObject":
            jsonData['GameObjects'].append({
                'modelName': gameObjectModel,
                'type': gameObjectType,
                'position': [translation[0], translation[1], translation[2]],
                'orientation': [orientation[0], orientation[1], orientation[2], orientation[3]],
                'scale': [1, 1, 1],
                'acceleration': gameObjectAcceleration,
                'velocity': gameObjectVelocity
            })
        elif gameObjectType == "EnemyTower":
            jsonData['GameObjects'].append({
                'modelName': gameObjectModel,
                'type': gameObjectType,
                'position': [translation[0], translation[1], translation[2]],
                'orientation': [orientation[0], orientation[1], orientation[2], orientation[3]],
                'scale': [1, 1, 1],
                'attackDamage': gameObjectAttackDamage,
                'attackDistance': gameObjectAttackDistance,
                'attackSpeed': gameObjectAttackSpeed,
                'health': gameObjectHealth,
                'maxHealth': gameObjectMaxHealth
            })
        elif gameObjectType == "EndLevelCollectable":
            jsonData['GameObjects'].append({
                'modelName': gameObjectModel,
                'type': gameObjectType,
                'position': [translation[0], translation[1], translation[2]],
                'orientation': [orientation[0], orientation[1], orientation[2], orientation[3]],
                'scale': [1, 1, 1]
            })

print orientation

print jsonData

with open('E:\\Documents\\Source Control Code Projects\\Game Engine Achitecture\\Game Engine Architecture\\GameEngineArchitecture\\resources\\scenes\\scene3.json', 'w') as outfile:
    json.dump(jsonData, outfile, sort_keys=True, indent=2)