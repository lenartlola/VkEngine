# VkEngine

> Build me a graphic engine worthy of Mordor...
>
> -- <cite> Sauron </cite>

## General Concepts

Since I started to get into the GPU world and tried to do the Vulkan triangle, I realised that it would be a draining
task to prepare the vulkan engine as I would do more GPU projects.

So I have decided to prepare an engine-like library which would serve as a template for each project, but I want it to
serve as more general purpose library so that other people can benefit it as well.

This project is under construction and isn't perfect by any mean, but I'm going to update it as I'm going to learn more
about vulkan intern working and I will make it customisable through configuration files.

This project doesn't implement the ValidationLayer at the current state, it would be the next step to take though.

There are a bunch of vulkan structure to fill when initialising vulkan instance and its queue families, it would be a
perfect shot to have a config file to fill these values.

## Device Compatibilities

I have not yet implemented the best algorithm to pick up the best resources, but it should clearly be done.
As for now I have some ideas how to do such a task, but I'll do it once I feel more comfortable with the vulkan intern.

## Tested on

This project has been tested on three different machines, 2 Macs and 1 Linux.

`Intel UHD 620` on Arch Linux BTW :P

`Intel Iris Plus` on MacBook Air Ventura 13.2

`AMD Radeon Pro 5300` on IMac mojave I think idk.
