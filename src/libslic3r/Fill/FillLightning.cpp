#include "../Print.hpp"
#include "../ShortestPath.hpp"

#include "FillLightning.hpp"
#include "Lightning/Generator.hpp"

namespace Slic3r::FillLightning {

void Filler::_fill_surface_single(
    const FillParams              &params,
    unsigned int                   thickness_layers,
    const std::pair<float, Point> &direction,
    const Polyline                  pedestal,
    ExPolygon                      expolygon,
    Polylines                     &polylines_out) const
{
    const Layer &layer = generator->getTreesForLayer(this->layer_id);
    Polylines    fill_lines = layer.convertToLines(to_polygons(expolygon), scaled<coord_t>(0.5 * this->get_spacing() - this->overlap));

    if (params.dont_connect() || fill_lines.size() <= 1) {
        append(polylines_out, chain_polylines(std::move(fill_lines)));
    } else
        connect_infill(std::move(fill_lines), expolygon, polylines_out, this->get_spacing(), params);
}

void GeneratorDeleter::operator()(Generator *p) {
    delete p;
}

GeneratorPtr build_generator(const PrintObject &print_object, const coordf_t fill_density, const std::function<void()> &throw_on_cancel_callback)
{
    return GeneratorPtr(new Generator(print_object, fill_density, throw_on_cancel_callback));
}

} // namespace Slic3r::FillAdaptive
