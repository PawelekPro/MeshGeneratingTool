/*
* Copyright (C) 2024 Krystian Fudali
*
* This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>
#include "Command.hpp"
#include "CommandStack.hpp"

class CommandStackTest : public ::testing::Test {
protected:
    CommandStack stack;
};

TEST_F(CommandStackTest, UndoOnEmptyStackDoesNothing) {
    EXPECT_EQ(stack.undoStackLength(), 0);
    stack.undo();
    EXPECT_EQ(stack.undoStackLength(), 0);
}

TEST_F(CommandStackTest, RedoOnEmptyStackDoesNothing) {
    EXPECT_EQ(stack.redoStackLength(), 0);
    stack.redo();
    EXPECT_EQ(stack.undoStackLength(), 0);
}

TEST_F(CommandStackTest, StacksExecuteCallsCommandsExecute) {
    auto command = std::make_unique<SpyCommand>();
    SpyCommand* rawCommand = command.get();

    stack.execute(std::move(command));
    EXPECT_EQ(rawCommand->nExecute(), 1);
}

TEST_F(CommandStackTest, StacksUndoCallsCommandsUndo) {
    auto command = std::make_unique<SpyCommand>();
    SpyCommand* rawCommand = command.get();

    stack.execute(std::move(command));
    stack.undo();
    EXPECT_EQ(rawCommand->nUndo(), 1);
}

TEST_F(CommandStackTest, StacksRedoCallsCommandsExecute) {
    auto command = std::make_unique<SpyCommand>();
    SpyCommand* rawCommand = command.get();

    stack.execute(std::move(command));
    stack.undo();
    stack.redo();

    EXPECT_EQ(rawCommand->nExecute(), 2);
}

TEST_F(CommandStackTest, LastCommandIsFirstToUndo) {
    auto command1 = std::make_unique<SpyCommand>();
    auto command2 = std::make_unique<SpyCommand>();
    SpyCommand* rawCommand1 = command1.get();
    SpyCommand* rawCommand2 = command2.get();

    stack.execute(std::move(command1));
    stack.execute(std::move(command2));

    stack.undo();
    
    EXPECT_EQ(rawCommand1->nUndo(), 0);
    EXPECT_EQ(rawCommand2->nUndo(), 1);
}

TEST_F(CommandStackTest, UndoPopsCommandFromUndoStack) {
    auto command = std::make_unique<SpyCommand>();

    stack.execute(std::move(command));
    EXPECT_EQ(stack.undoStackLength(), 1);
    stack.undo();
    EXPECT_EQ(stack.undoStackLength(), 0);
}

TEST_F(CommandStackTest, LastUndoneCommandIsFirstToRedo) {
    auto command1 = std::make_unique<SpyCommand>();
    auto command2 = std::make_unique<SpyCommand>();
    SpyCommand* rawCommand1 = command1.get();
    SpyCommand* rawCommand2 = command2.get();

    stack.execute(std::move(command1));
    stack.execute(std::move(command2));

    stack.undo();
    stack.redo();

    EXPECT_EQ(rawCommand1->nExecute(), 1);
    EXPECT_EQ(rawCommand2->nExecute(), 2);
}

TEST_F(CommandStackTest, RedoPopsCommandFromRedoStack) {
    auto command = std::make_unique<SpyCommand>();

    stack.execute(std::move(command));
    stack.undo();
    EXPECT_EQ(stack.redoStackLength(), 1);
    stack.redo();
    EXPECT_EQ(stack.redoStackLength(), 0);
}