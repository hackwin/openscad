/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "Children.h"
#include "module.h"
#include "ModuleInstantiation.h"
#include "Parameters.h"
#include "printutils.h"
#include "Builtins.h"

#include "TextNode.h"
#include "FreetypeRenderer.h"

#include <boost/assign/std/vector.hpp>
using namespace boost::assign; // bring 'operator+=()' into scope

static std::shared_ptr<AbstractNode> builtin_text(const ModuleInstantiation *inst, Arguments arguments, Children children)
{
  if (!children.empty()) {
    LOG(message_group::Warning, inst->location(), arguments.documentRoot(),
        "module %1$s() does not support child modules", inst->name());
  }

  auto node = std::make_shared<TextNode>(inst);

  Parameters parameters = Parameters::parse(std::move(arguments), inst->location(),
                                            {"text", "size", "font"},
                                            {"direction", "language", "script", "halign", "valign", "spacing"}
                                            );
  parameters.set_caller("text");

  node->params.set_loc(inst->location());
  node->params.set_documentPath(arguments.documentRoot());

  node->params.set(parameters);
  node->params.detect_properties();

  return node;
}

std::vector<const Geometry *> TextNode::createGeometryList() const
{
  FreetypeRenderer renderer;
  return renderer.render(this->get_params());
}

FreetypeRenderer::Params TextNode::get_params() const
{
  return params;
}

std::string TextNode::toString() const
{
  return STR(name(), "(", this->params, ")");
}

void register_builtin_text()
{
  Builtins::init("text", new BuiltinModule(builtin_text),
  {
    "text(text = \"\", size = 10, font = \"\", halign = \"left\", valign = \"baseline\", spacing = 1, direction = \"ltr\", language = \"en\", script = \"latin\"[, $fn])",
  });
}
