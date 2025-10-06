//
//  main.cpp
//  cpp_project
//
//  Created by aibolat on 06.10.2025.
//

#include <cstring>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_metal.h"
#include "imgui/imgui.h"

#import "Cocoa/Cocoa.h"
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "pupsi/pupsi.hpp"
#include "pupsi/pupsimemory.hpp"

int main(int argc, const char* argv[])
{
        
        if (!glfwInit())
                return 1;
        
        NSRect screenrect = [[NSScreen mainScreen] frame];
        int screenwidth = (int)screenrect.size.width;
        int screenheight = (int)screenrect.size.height;
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        
        float main_scale
        = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(screenwidth, screenheight, "Dear ImGui GLFW+Metal example", NULL, NULL);
        if (window == nullptr)
                return 1;
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;
        
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        id<MTLCommandQueue> commandQueue = [device newCommandQueue];
        
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplMetal_Init(device);
        NSWindow* nswin = glfwGetCocoaWindow(window);
        [nswin setOpaque:NO];
        [nswin setLevel:NSMainMenuWindowLevel + 2];
        [nswin setIgnoresMouseEvents:YES];
        
        CAMetalLayer* layer = [CAMetalLayer layer];
        layer.opaque = NO;
        layer.device = device;
        layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        nswin.contentView.layer = layer;
        nswin.contentView.wantsLayer = YES;
        
        [nswin setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces
         | NSWindowCollectionBehaviorFullScreenAuxiliary |
         NSWindowCollectionBehaviorIgnoresCycle];
        
        MTLRenderPassDescriptor* renderPassDescriptor = [MTLRenderPassDescriptor new];
        renderPassDescriptor.colorAttachments[0].clearColor
        = MTLClearColorMake(0, 0, 0, 0);
        
        bool overlay_gui = true;
        while (!glfwWindowShouldClose(window)) {
                @autoreleasepool {
                        
                        glfwPollEvents();
                        
                        int width, height;
                        glfwGetFramebufferSize(window, &width, &height);
                        layer.drawableSize = CGSizeMake(width, height);
                        id<CAMetalDrawable> drawable = [layer nextDrawable];
                        
                        id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
                        
                        renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
                        renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
                        renderPassDescriptor.colorAttachments[0].storeAction
                        = MTLStoreActionStore;
                        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
                        
                        ImGui_ImplMetal_NewFrame(renderPassDescriptor);
                        ImGui_ImplGlfw_NewFrame();
                        ImGui::NewFrame();
                        
                        if (overlay_gui) {
                                static float f = 0.0f;
                                
                                static mach_vm_address_t imagebase;
                                
                                ImGui::Begin("Overlay test");
                                ImGui::Text("struggle with metal");
                                
                                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
                                
                                if (ImGui::Button("try inject"))
                                        pupsi::tryinject();
                                
                                if (ImGui::Button("getimagebase"))
                                        pupsi::tryreadvisualengine();
                                
                                ImGui::Text("base address is: %llx", imagebase);
                                
                                ImGui::End();
                        }
                        
                        ImGui::Render();
                        bool imguihovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)
                        || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive();
                        static bool lastignores = true;
                        bool wantignore = !imguihovered;
                        if (wantignore != lastignores) {
                                [nswin setIgnoresMouseEvents:wantignore];
                                lastignores = wantignore;
                        }
                        
                        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);
                        [renderEncoder endEncoding];
                        
                        [commandBuffer presentDrawable:drawable];
                        [commandBuffer commit];
                }
        }
        
        ImGui_ImplMetal_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        glfwDestroyWindow(window);
        glfwTerminate();
        
        // 0x100CA6238
        
        return EXIT_SUCCESS;
}
