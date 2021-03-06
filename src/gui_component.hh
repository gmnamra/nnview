#ifndef NNVIEW_GUI_COMPONENT_HH_
#define NNVIEW_GUI_COMPONENT_HH_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "imgui_node_editor.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include "datatypes.h"

#include <string>
#include <vector>
#include <map>

namespace ed = ax::NodeEditor;

namespace nnview {

struct ImNode;

enum class PinType {
  Flow,
  Bool,
  Int,
  Float,
  String,
  Object,
  Function,
  Delegate,
};

enum class PinKind { Output, Input };

struct Link {
  ed::LinkId ID;

  ed::PinId StartPinID;
  ed::PinId EndPinID;

  ImColor Color;

  Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId)
      : ID(id),
        StartPinID(startPinId),
        EndPinID(endPinId),
        Color(255, 255, 255) {}
};

struct Pin {
  ed::PinId ID;
  ImNode *ImNode;
  std::string Name;
  PinType Type;
  PinKind Kind;

  Pin(ed::PinId id, const std::string name, PinType type)
      : ID(id), ImNode(nullptr), Name(name), Type(type), Kind(PinKind::Input) {}
};

struct ImNode {
  // For imgui-node-editor
  ed::NodeId id;
  std::string name;
  std::vector<Pin> inputs;
  std::vector<Pin> outputs;
  ImColor color;
  ImVec2 size;

  int tensor_id = -1;  // Index to nnview::Graph::tensors

  ImNode(ed::NodeId _id, const std::string _name,
         ImColor _color = ImColor(255, 255, 255))
      : id(_id), name(_name), color(_color), size(0, 0) {}
};

class GUIContext {
 public:
  int _active_tensor_idx = -1; // index to nnview::Graph::tensors

  nnview::Graph _graph;

  // Node and Link(connection) information using imgui-node-editor
  std::vector<ImNode> _imnodes;
  std::vector<Link> _links;

  std::map<int, int> _node_id_to_imnode_idx_map; // <NodeId, index to _imnodes>

  // OpenGL texture id for displaying Tensor as Texture(Image)
  std::vector<GLuint> _tensor_texture_ids;

  GLuint _background_texture_id = 0;

  ed::EditorContext *_editor_context = nullptr;

  // Call `init` before calling any methods defined in `GUIContext`.
  // `graph` variable must be set before calling `init`.
  void init();

  // Initialize and layout ImNodes from Graph.
  // This function should be called after `init` and before calling ImNode
  // drawing methods.
  void init_imnode_graph();

  void draw_imnodes();

  // Draw Tensor in active section.
  void draw_tensor();

  void finalize();
};

}  // namespace nnview

#endif  // NNVIEW_GUI_COMPONENT_HH_
