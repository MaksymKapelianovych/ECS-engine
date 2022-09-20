// #include "collision.h"
//
// #include <iostream>
//
// #include "interactionWorld.hpp"
//
// Map<CollisionTypePair, CollisionResponse> InteractionWorld::collisionTable =
//     {
//         {{CollisionType::STATIC, CollisionType::STATIC}, CollisionResponse::NONE},
//         {{CollisionType::STATIC, CollisionType::MOVABLE}, CollisionResponse::BLOCK},
//         {{CollisionType::STATIC, CollisionType::PLAYER}, CollisionResponse::BLOCK},
//         {{CollisionType::STATIC, CollisionType::NONE}, CollisionResponse::BLOCK},
//     
//         {{CollisionType::MOVABLE, CollisionType::MOVABLE}, CollisionResponse::NONE},
//         {{CollisionType::MOVABLE, CollisionType::PLAYER}, CollisionResponse::BLOCK},
//         {{CollisionType::MOVABLE, CollisionType::NONE}, CollisionResponse::NONE},
//     
//         {{CollisionType::PLAYER, CollisionType::PLAYER}, CollisionResponse::BLOCK},
//         {{CollisionType::PLAYER, CollisionType::NONE}, CollisionResponse::NONE},
//     
//         {{CollisionType::NONE, CollisionType::NONE}, CollisionResponse::NONE},
//     };