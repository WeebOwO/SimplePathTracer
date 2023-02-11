#pragma once

#include <memory>

#include "geometry.h"
#include "misc.h"


struct Pdf {
    virtual float Value(const glm::vec3& direction) const = 0;
    virtual glm::vec3 Generate() const = 0;    
};


struct CosinePdf : public Pdf {
    CosinePdf(const glm::vec3& w) {
        uvw.BuildFromW(w);
    }

    virtual float Value(const glm::vec3& direction) const override {
        float cosine = glm::dot(glm::normalize(direction), uvw.w);
        return (cosine <= 0) ? 0 : cosine / pbr::pi;
    }

    virtual glm::vec3 Generate() const override {
        return uvw.Local(pbr::RandomCosineDirection());
    };
    Onb uvw;
};
