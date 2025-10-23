### VK_EXT_debug_utils

The VK_EXT_debug_utils is an instance-level extension that helps devs see what their Vulkan app is doing internally

* It does so by giving the developer three main powers
  
  * Debug Messenger, which received infomation from the validation layers (info like warnings, errors, etc)
  
  * Object Naming, Gives meaningfull names to GPU objects (buffers, images, pipelines, etc)
  
  * Debug Levels, marks command buffers and queues with colorful labels for debugging purposes

* Now, let's define what a callback is: A callback is simply a function that you write, but the system calls it when the event happens. So, this function is only ever called when an event occurs
  
  * In our case, the callback will be called when the validation layers detect a warning, or an error.
  
  * So, instead of Vulkan printing the error to the console automatically which is never does, you give it a function pointer to the callback function. 
  
  * So, everytime the validation layers detect a message Vulkan calls the function.

* To set the debug_utils up, we need to:
  
  * Enable the extension,  when creating the instance, in my case, I did so in my *instance.cpp* file. You could enable it by using **VK_EXT_DEBUG_UTILS_EXTENSION_NAME**
  * Create a debug messenger, which connects Vulkan to the callback; we do so by using a  **vk::DebugUtilsMessengerCreateInfoEXT**
  
  ### Beyond the callback, this function adds two more key features that make RenderDoc and validation logs understandable.
  
  ##### Object Naming `vk::setDebugUtilsObjectNameEXT`
  
  * Every Vulkan object (like buffers, shaders, or pipelines) is just a 64-bit handle -- making it meaningless in RenderDoc logs
  
  * We can give them a readable name like this 
  
  * ```
    vk::setDebugUtilsObjectNameEXT info {
        sType,
        objectType, # this could be VK_OBJECT_TYPE_BUFFER
        objectHandle, # since all Vulkan objects are just 64-bit handles we pass the object itself here
        pObjectName, # The name of object 
    }
    vk::setDebugUtilsObjectNameEXT(device, &info)
    ```

#### Labels (markers) for debugging GPU work

* RenderDoc shows all the commands, but without context they're are just a list of draw calls

* So, we can use `vk::debugUtilsLabelEXT ` to add markers and colored sections

* ```
  VkDebugUtilsLabelEXT labelInfo{};
  labelInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
  labelInfo.pLabelName = "Shadow Pass";
  labelInfo.color[0] = 1.0f;
   labelInfo.color[1] = 0.5f; 
  labelInfo.color[2] = 0.0f;
   labelInfo.color[3] = 1.0f;
  vkCmdBeginDebugUtilsLabelEXT(cmdBuffer, &labelInfo);
  // ... record shadow pass commands ...
  vkCmdEndDebugUtilsLabelEXT(cmdBuffer);
  ```

| Function                                                          | Purpose                                       |
| ----------------------------------------------------------------- | --------------------------------------------- |
| `vkCreateDebugUtilsMessengerEXT`                                  | Registers your validation message callback.   |
| `vkDestroyDebugUtilsMessengerEXT`                                 | Removes the callback.                         |
| `vkSetDebugUtilsObjectNameEXT`                                    | Give readable names to Vulkan objects.        |
| `vkSetDebugUtilsObjectTagEXT`                                     | Attach arbitrary binary data to objects.      |
| `vkCmdBeginDebugUtilsLabelEXT` / `vkCmdEndDebugUtilsLabelEXT`     | Mark sections in command buffers.             |
| `vkCmdInsertDebugUtilsLabelEXT`                                   | Insert a one-shot marker in a command buffer. |
| `vkQueueBeginDebugUtilsLabelEXT` / `vkQueueEndDebugUtilsLabelEXT` | Label work submitted to a queue.              |
| `vkQueueInsertDebugUtilsLabelEXT`                                 | Insert markers in queue submissions.          |
