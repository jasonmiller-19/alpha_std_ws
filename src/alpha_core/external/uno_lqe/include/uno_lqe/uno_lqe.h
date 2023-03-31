#ifndef UNO_LQE_UNO_LQE_H
#define UNO_LQE_UNO_LQE_H

#include "memory"

class UnoLqe {

private:

    //! @brief State model
    double m_A;

    //! @brief Input model
    double m_B;

    //! @brief Observation model
    double m_H;

    //! @brief Process noise, variance
    double m_Q;

    //! @brief Measurement noise, variance
    double m_R;

    //! @brief Process variance
    double m_P;

    //! @brief System State
    double m_x;

public:

    UnoLqe(double x, double A, double B, double H, double Q, double R, double P);

    void initialize(double x, double A, double B, double H, double Q, double R, double P);

    void predict();

    void predict(double u);

    void update(double z);

    void state(double *x, double *p) const;

    typedef std::shared_ptr<UnoLqe> Ptr;
};

#endif //UNO_LQE_UNO_LQE_H
