#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

std::string ftofstr(float num, int decimal) { // slightly changed the float formatting thing ive been using all this time
    if (decimal == 0) return std::to_string((int)round(num));
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(decimal) << num;
    std::string string = ss.str();
    string.erase(string.find_last_not_of('0') + 1, std::string::npos);
    if (string.back() == '.') string.pop_back();
    return string;
}

class $modify(LevelLayer, LevelInfoLayer) {
    struct Fields {
        CCLabelBMFont* percentLikedLabel;
    };
    
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        auto percentLikedLabel = CCLabelBMFont::create("", "bigFont.fnt");
        percentLikedLabel->setColor(ccc3(255, 255, 255));
        percentLikedLabel->setPosition(m_likesLabel->getPositionX(), m_likesLabel->getPositionY() - 12);
        percentLikedLabel->setAnchorPoint(ccp(0.0f, 0.5f));
        percentLikedLabel->setScale(0.3f);
        this->addChild(percentLikedLabel);
        m_fields->percentLikedLabel = percentLikedLabel;
        updatePercentLikedLabel();
        return true;
    }

    void levelDownloadFinished(GJGameLevel* p0) { // not sure if this is needed but just to be safe
        LevelInfoLayer::levelDownloadFinished(p0);
        updatePercentLikedLabel();
    }

    void updatePercentLikedLabel() {
        auto label = m_fields->percentLikedLabel;
        auto likes = m_level->m_likes;
        auto downloads = m_level->m_downloads;
        auto percent = (likes / (float)downloads) * 100;
        if (!label) return;
        if (downloads == 0) {
            label->setString("N/A");
            return;
        }
        if (percent < 0){
            m_fields->percentLikedLabel->setColor(ccc3(255, 0, 0));
        }
        else if (percent > 0){
            m_fields->percentLikedLabel->setColor(ccc3(0, 255, 0));
        }
        label->setString((ftofstr(percent, 2) + "%").c_str());
    }
};
