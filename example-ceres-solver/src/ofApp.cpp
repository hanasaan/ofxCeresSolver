// this example is heavily reffered from
// https://github.com/elliotwoods/ofxCeres/tree/master/Example-RigidBody
#include "ofxCeresSolver.h"

#include "ofMain.h"

struct RigidBodyTransformError {
    RigidBodyTransformError(const glm::tvec3<double> & untransformedPoint, const glm::tvec3<double> & transformedPoint)
    : untransformedPoint(untransformedPoint)
    , transformedPoint(transformedPoint) {}
    
    template <typename T>
    bool operator()(const T * const transformParameters
                    , T * residuals) const {
        
        glm::tvec3<T> translation(transformParameters[0], transformParameters[1], transformParameters[2]);
        glm::tvec3<T> rotationVector(transformParameters[3], transformParameters[4], transformParameters[5]);
        
        auto transform = ofxCeresSolver::VectorMath::createTransform(translation, rotationVector);
        auto predictedTransformedPoint = transform * glm::tvec4<T>(this->untransformedPoint, 1.0);
        
        predictedTransformedPoint /= predictedTransformedPoint.w;
        
        for (int i = 0; i < 3; i++) {
            residuals[i] = this->transformedPoint[i] - predictedTransformedPoint[i];
        }
        
        return true;
    }
    
    static ceres::CostFunction * Create(const glm::tvec3<double> & untransformedPoint, const glm::tvec3<double> & transformedPoint) {
        return (new ceres::AutoDiffCostFunction<RigidBodyTransformError, 3, 6>(
                                                                               new RigidBodyTransformError(untransformedPoint, transformedPoint)));
    }
    
    glm::tvec3<double> untransformedPoint;
    glm::tvec3<double> transformedPoint;
};


class ofApp : public ofBaseApp{
    vector<glm::vec3> untransformedPoints;
    vector<glm::vec3> transformedPoints;
    glm::mat4 solvedTransform;
    
    ofEasyCam camera;
    
    float noise = 3.0;
    float scale = 100.0;
public:
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
        
        this->randomizeTransform();
        this->solve();
        
    }
    
    void randomizeTransform() {
        this->untransformedPoints.clear();
        this->transformedPoints.clear();
        
        // Create a random transform
        auto translation = glm::vec3(ofRandomf(), ofRandomf(), ofRandomf()) * scale;
        auto rotationVector = glm::vec3(ofRandomf(), ofRandomf(), ofRandomf());
        auto transform = ofxCeresSolver::VectorMath::createTransform(translation, rotationVector);

        // Synthesise some data
        
        for (int i = 0; i < 1000; i++) {
            auto untransformedPoint = glm::vec3(ofRandomf(), ofRandomf(), ofRandomf()) * scale;
            auto transformedPoint = glm::vec3(transform * glm::vec4(untransformedPoint, 1.0));
            
            transformedPoint += glm::vec3(ofRandomf(), ofRandomf(), ofRandomf()) * noise;
            
            untransformedPoints.push_back(untransformedPoint);
            transformedPoints.push_back(transformedPoint);
        }
    }
    
    void update()
    {
    }
    
    void solve()
    {
        auto ts = ofGetElapsedTimef();
        
        double parameters[6];
        for (auto & parameter : parameters) {
            parameter = 0.0;
        }
        
        ceres::Problem problem;
        size_t size = untransformedPoints.size();
        for (size_t i = 0; i < size; i++) {
            ceres::CostFunction * costFunction = RigidBodyTransformError::Create(untransformedPoints[i], transformedPoints[i]);
            problem.AddResidualBlock(costFunction
                                     , NULL
                                     , parameters);
        }
        
        ceres::Solver::Options options;
        options.linear_solver_type = ceres::DENSE_SCHUR;
        options.minimizer_progress_to_stdout = false;//true;
        ceres::Solver::Summary summary;
        ceres::Solve(options, &problem, &summary);
//        std::cout << summary.FullReport() << "\n";
        
        // construct result
        glm::vec3 translation(parameters[0], parameters[1], parameters[2]);
        glm::vec3 rotationVector(parameters[3], parameters[4], parameters[5]);
        auto transform = ofxCeresSolver::VectorMath::createTransform(translation, rotationVector);
        solvedTransform = transform;
        
        auto te = ofGetElapsedTimef();
        cerr << "solved in " << (te - ts) * 1000.0 << "msec" << endl;
    }
    
    void draw()
    {
        ofClear(0);
        
        this->camera.begin();
        {
            ofEnableDepthTest();
            
            ofDrawGrid(10.0f);
            
            ofPushStyle();
            {
                //draw untransformed points
                ofSetColor(200, 100, 100);
                for (const auto & point : this->untransformedPoints) {
                    ofDrawSphere(point, 1);
                }
                
                //draw transformed points
                ofSetColor(100, 200, 100);
                for (const auto & point : this->transformedPoints) {
                    ofDrawSphere(point, 1);
                }
                
                //draw untransformed point * predicted transform
                ofSetColor(100, 100, 200);
                ofNoFill();
                ofPushMatrix();
                {
                    ofMultMatrix(this->solvedTransform);
                    for (const auto & point : this->untransformedPoints) {
                        ofDrawCircle(point, 2);
                    }
                }
                ofPopMatrix();
            }
            ofPopStyle();
            
            ofDisableDepthTest();
        }
        this->camera.end();
    }
    
    void keyPressed(int key)
    {
        if (key == OF_KEY_RETURN) {
            this->solve();
        }
    }
};

//========================================================================
int main( ){
    ofSetupOpenGL(1280,720,OF_WINDOW);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
