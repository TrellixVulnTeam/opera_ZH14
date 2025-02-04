/*
 * Copyright (C) 2004, 2005, 2008 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006 Rob Buis <buis@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SVGStyledLocatableElement_h
#define SVGStyledLocatableElement_h

#include "core/svg/SVGLocatable.h"
#include "core/svg/SVGStyledElement.h"

namespace WebCore {

class SVGElement;

class SVGStyledLocatableElement : public SVGStyledElement,
                                  virtual public SVGLocatable {
public:
    virtual SVGElement* nearestViewportElement() const;
    virtual SVGElement* farthestViewportElement() const;

    virtual FloatRect getBBox(StyleUpdateStrategy = AllowStyleUpdate);
    virtual AffineTransform getCTM(StyleUpdateStrategy = AllowStyleUpdate);
    virtual AffineTransform getScreenCTM(StyleUpdateStrategy = AllowStyleUpdate);

    virtual AffineTransform localCoordinateSpaceTransform(SVGLocatable::CTMScope mode) const { return SVGLocatable::localCoordinateSpaceTransform(mode); }

protected:
    SVGStyledLocatableElement(const QualifiedName&, Document*, ConstructionType = CreateSVGElement);

private:
    virtual bool isStyledLocatable() const OVERRIDE { return true; }
};

inline SVGStyledLocatableElement* toSVGStyledLocatableElement(SVGElement* element)
{
    ASSERT_WITH_SECURITY_IMPLICATION(!element || element->isStyledLocatable());
    return static_cast<SVGStyledLocatableElement*>(element);
}

} // namespace WebCore

#endif // SVGStyledLocatableElement_h
