#include "canvas.hpp"
#include "util/text_data.hpp"
#include "util/str_util.hpp"
#include "util/geom_util.hpp"
#include <algorithm>
#include <glibmm.h>

namespace horizon {

std::pair<Coordf, Coordf> Canvas::draw_text(const Coordf &p, float size, const std::string &rtext, int angle,
                                            TextOrigin origin, ColorP color, int layer, const TextOptions &opts)
{
    if (img_mode)
        img_draw_text(p, size, rtext, angle, opts.flip, origin, layer, opts.width, opts.font, opts.center, opts.mirror);
    angle = wrap_angle(angle);
    bool backwards = (angle > 16384) && (angle <= 49152) && !opts.allow_upside_down;
    float sc = size / 21;
    float yshift = 0;
    switch (origin) {
    case TextOrigin::CENTER:
        yshift = -21 / 2;
        break;
    case TextOrigin::BOTTOM:
        yshift = 21 / 2;
        break;
    default:
        yshift = 0;
    }
    Coordf a = p;
    Coordf b = p;

    std::string text(rtext);
    trim(text);
    std::stringstream ss(text);
    std::string line;
    unsigned int n_lines = std::count(text.begin(), text.end(), '\n');
    unsigned int i_line = 0;
    float lineskip = size * 1.35 + opts.width;
    if (opts.mirror) {
        lineskip *= -1;
    }
    begin_group(layer);
    while (std::getline(ss, line, '\n')) {
        const TextData td{text_data_buffer, line, opts.font};

        Placement tf;
        tf.shift.x = p.x;
        tf.shift.y = p.y;

        Placement tr;
        if (opts.flip)
            tr.set_angle(32768 - angle);
        else
            tr.set_angle(angle);
        if ((backwards ^ opts.mirror) ^ opts.flip)
            tf.shift += tr.transform(Coordi(0, -lineskip * (n_lines - i_line)));
        else
            tf.shift += tr.transform(Coordi(0, -lineskip * i_line));

        int xshift = 0;
        if (backwards) {
            tf.set_angle(angle - 32768);
            xshift = -td.xright;
        }
        else {
            tf.set_angle(angle);
        }
        tf.mirror = opts.flip;
        if (opts.center) {
            if (backwards) {
                xshift += td.xright / 2;
            }
            else {
                xshift -= td.xright / 2;
            }
        }


        for (const auto &li : td.lines) {
            Coordf o0 = li.first;
            o0.x += xshift;
            o0.y += yshift;
            o0 *= sc;

            Coordf o1 = li.second;
            o1.x += xshift;
            o1.y += yshift;
            o1 *= sc;

            Coordf p0 = tf.transform(o0);
            Coordf p1 = tf.transform(o1);
            a = Coordf::min(a, Coordf::min(p0, p1));
            b = Coordf::max(b, Coordf::max(p0, p1));
            if (opts.draw) {
                img_line(Coordi(p0.x, p0.y), Coordi(p1.x, p1.y), opts.width, layer, false);
                if (!img_auto_line)
                    draw_line(p0, p1, color, layer, false, opts.width);
            }
        }
        i_line++;
    }
    end_group();

    Coordf e(opts.width / 2, opts.width / 2);
    return {a - e, b + e};
}
} // namespace horizon
