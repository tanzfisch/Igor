// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "GameInput.h"

#include "GameControl.h"

#include <iKeyboard.h>
using namespace Igor;

GameInput::GameInput(GameControl* gameControl)
: _gameControl(gameControl)
{
    _player = _gameControl->getPlayer();

    iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveUpPlayer), iKeyCode::Up);
    iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveDownPlayer), iKeyCode::Down);
    iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveLeftPlayer), iKeyCode::Left);
    iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveRightPlayer), iKeyCode::Right);
    iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMovePrimaryActionPlayer), iKeyCode::Space);
    iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveSecondaryActionPlayer), iKeyCode::C);

    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveUpPlayer), iKeyCode::Up);
    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveDownPlayer), iKeyCode::Down);
    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveLeftPlayer), iKeyCode::Left);
    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveRightPlayer), iKeyCode::Right);
    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMovePrimaryActionPlayer), iKeyCode::Space);
    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveSecondaryActionPlayer), iKeyCode::C);

    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegate(this, &GameInput::exitGame), iKeyCode::ESC);

}

GameInput::~GameInput()
{
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveUpPlayer), iKeyCode::Up);
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveDownPlayer), iKeyCode::Down);
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveLeftPlayer), iKeyCode::Left);
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveRightPlayer), iKeyCode::Right);
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMovePrimaryActionPlayer), iKeyCode::Space);
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegate(this, &GameInput::startMoveSecondaryActionPlayer), iKeyCode::C);

    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveUpPlayer), iKeyCode::Up);
    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveDownPlayer), iKeyCode::Down);
    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveLeftPlayer), iKeyCode::Left);
    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveRightPlayer), iKeyCode::Right);
    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMovePrimaryActionPlayer), iKeyCode::Space);
    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegate(this, &GameInput::stopMoveSecondaryActionPlayer), iKeyCode::C);

    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegate(this, &GameInput::exitGame), iKeyCode::ESC);
}

void GameInput::exitGame()
{
    _gameControl->exitGame();
}

void GameInput::startMoveUpPlayer()
{
    _gameControl->activateEntityAction(_player, GameAction::MoveUp);
}

void GameInput::stopMoveUpPlayer()
{
    _gameControl->deactivateEntityAction(_player, GameAction::MoveUp);
}

void GameInput::startMoveDownPlayer()
{
    _gameControl->activateEntityAction(_player, GameAction::MoveDown);
}

void GameInput::stopMoveDownPlayer()
{
    _gameControl->deactivateEntityAction(_player, GameAction::MoveDown);
}

void GameInput::startMoveLeftPlayer()
{
    _gameControl->activateEntityAction(_player, GameAction::MoveLeft);
}

void GameInput::stopMoveLeftPlayer()
{
    _gameControl->deactivateEntityAction(_player, GameAction::MoveLeft);
}

void GameInput::startMoveRightPlayer()
{
    _gameControl->activateEntityAction(_player, GameAction::MoveRight);
}

void GameInput::stopMoveRightPlayer()
{
    _gameControl->deactivateEntityAction(_player, GameAction::MoveRight);
}

void GameInput::startMovePrimaryActionPlayer()
{
    _gameControl->activateEntityAction(_player, GameAction::PrimaryAction);
}

void GameInput::stopMovePrimaryActionPlayer()
{
    _gameControl->deactivateEntityAction(_player, GameAction::PrimaryAction);
}

void GameInput::startMoveSecondaryActionPlayer()
{
    _gameControl->activateEntityAction(_player, GameAction::SecondaryAction);
}

void GameInput::stopMoveSecondaryActionPlayer()
{
    _gameControl->deactivateEntityAction(_player, GameAction::SecondaryAction);
}
