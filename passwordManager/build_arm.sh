#!/bin/bash

../../sdk.py --function_definitions function_definitions.json \
--system_configuration ../../default_system_config.json \
--program_name password_manager --compilation_config compilation_config.json \
--build_arm
