﻿#include "pch.h"

using namespace winrt;
using namespace Windows::Foundation;

int main()
{
    init_apartment();
    auto x = qgl::bridge::FolderExists();
}
