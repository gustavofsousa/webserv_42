#!/usr/bin/env python3
import os

env_var = os.environ

for key, value in env_var.items():
    print(f"{key} = {value}")