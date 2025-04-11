from SCons.Script import *
Import("env")

def after_build(source, target, env):
    env.Execute("arm-none-eabi-objcopy -O ihex .pio/build/%s/firmware.elf .pio/build/%s/firmware.hex" % (env["PIOENV"], env["PIOENV"]))

env.AddPostAction("buildprog", after_build)