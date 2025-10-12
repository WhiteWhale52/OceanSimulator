### Day 4



### Stuff achieved on this Day

* Today the task is to make the vulkan application and instance classes.

* I created an instance using vk::instanceCreateInfo struct which I passed into vk::CreateInstance function.

* Most of the code is based of  `GetIntoGameDev` Vulkan with C++ series

* I copied his debugging code, in order to make all errors as obvious as possible.

* We can now create a glfw window, extending it with imgui will occur on a later date.

* I added the imgui code

* I added validation layers and the glfw extensions using glfwGetRequiredInstanceExtensions

* I learnt about vk::ReturnCode and how to use try and vk::SystemError to print an error message on the console

* Vulkan holds its version number in 32 bit integers, where the Variant, Patch, Major, Minor of the versions can all be identified by calling VK_API_VERSION_* where * can be PATCH, VARIANT, MAJOR, or MINOR.

* Following the adding validation layers tutorial, I added printing of all the supported layers and extension of the instance

* I also added VK_LAYER_KHRONOS_validation to the layers and VK_EXT_debug_utils

* Created an instance of vk::DebugUtilsMessengerEXT to the engine.h file

### Personal Notes:

1. Vulkan is designed to be as thin as possible, so no built-in bookkeeping or error-handling, that is why was use ValidationLayers to detect invalid usuage and manually check return codes or enable extensions for error reporting.

2. A further supporting point to the one above, is that the philosophy behind Vulkan is to put as much responsibility on the developer as possible to achieve maximum performance. 

3. So, Vulkan requires the developer to be very explicit with what they are doing and so fine-grained control is permitted, unlike in OpenGL. 
   
   * But this opens the door for many potension small mistakes from the developer due to unfamilarity with the architecture.

4. 


