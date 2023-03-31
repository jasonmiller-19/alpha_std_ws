#include "uno_lqe/uno_lqe.h"

UnoLqe::UnoLqe(double x, double A, double B, double H, double Q, double R, double P) {
    initialize(x, A, B, H, Q, R, P);
}

void UnoLqe::initialize(double x, double A, double B, double H, double Q, double R, double P) {
    m_A = A;
    m_B = B;
    m_H = H;
    m_P = P;
    m_Q = Q;
    m_R = R;
    m_x = x;
}

void UnoLqe::predict() {
    predict(0);
}

void UnoLqe::predict(double u) {
    m_x = m_A * m_x + m_B * u;
    m_P = m_A * m_P * m_A + m_Q;
}

void UnoLqe::update(double z) {
    double K = m_P * m_H * ( 1.0 / (m_H * m_P * m_H + m_R));
    m_x = m_x + K * (z - m_H * m_x);
    m_P = (1 - K * m_H) * m_P;
}

void UnoLqe::state(double *x, double *p) const {
    *x = m_x;
    *p = m_P;
}